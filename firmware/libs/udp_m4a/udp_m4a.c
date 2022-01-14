#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "thread.h"
#include "udp_m4a.h"

#define SERVER_MSG_QUEUE_SIZE   (8)
#define SERVER_BUFFER_SIZE      (100)

bool server_running = false;
sock_udp_t sock;
uint8_t server_buffer[SERVER_BUFFER_SIZE];
char server_stack[THREAD_STACKSIZE_DEFAULT];
msg_t server_msg_queue[SERVER_MSG_QUEUE_SIZE];

void *_udp_serverf(void *args)
{
    udpf_payload* castPayload =  (udpf_payload*)args;
    sock_udp_ep_t server = { .port = atoi(castPayload->port), .family = AF_INET6 };
    msg_init_queue(server_msg_queue, SERVER_MSG_QUEUE_SIZE);

    if(sock_udp_create(&sock, &server, NULL, 0) < 0) {
        return NULL;
    }

    server_running = true;
    printf("Success: started UDP server on port %u\n", server.port);
    while(1) {
        int res;
        if ((res = sock_udp_recv(&sock, server_buffer,
                                 sizeof(server_buffer) - 1, SOCK_NO_TIMEOUT,
                                 NULL)) < 0) {
            puts("Error while receiving");
        }
        else if (res == 0) {
            puts("No data received");
        }
        else {
            castPayload->callback(server_buffer, res);
        }
    }

    return NULL;
}

int udp_send(int* port, char* address, uint8_t* message, size_t* payload_len)
{
    int res;
    sock_udp_ep_t remote = { .family = AF_INET6 };

    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, address) == NULL) {
        puts("Error: unable to parse destination address");
        return 1;
    }
    if (ipv6_addr_is_link_local((ipv6_addr_t *)&remote.addr)) {
        /* choose first interface when address is link local */
        gnrc_netif_t *netif = gnrc_netif_iter(NULL);
        remote.netif = (uint16_t)netif->pid;
    }
    remote.port = *port;
    if((res = sock_udp_send(NULL, message, *payload_len, &remote)) < 0) {
        puts("could not send");
    }
    else {
        printf("Success: send %u bytes  from %s \n", (unsigned) res, address);
    }
    return 0;
}

int udp_server(udpf_payload* argv)
{
    if ((server_running == false) &&
        thread_create(server_stack, sizeof(server_stack), THREAD_PRIORITY_MAIN - 1,
                      THREAD_CREATE_STACKTEST, _udp_serverf, argv, "UDP Server")
        <= KERNEL_PID_UNDEF) {
        return -1;
    }

    return 0;
}
