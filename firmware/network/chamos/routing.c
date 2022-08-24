/*
 * Copyright (c) 2022 Mesh4all <mesh4all.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * @brief   CHAMOS  Communication Handler for Addressing Management | Origin Server
 *
 * @author  RocioRojas <Rociorojas391@gmail.com>
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "net_tools.h"
#include "chamos.h"
#include "radio.h"
#include "net/gnrc/ipv6/nib/ft.h"
#include "xtimer.h"

#define CHAMOS_MSG_QUEUE_SIZE (8)
#define SERVER_BUFFER_SIZE (CONFIG_SERVER_BUFFER_SIZE)

sock_udp_t chamosr_sock = {};
char chamosr_stack[THREAD_STACKSIZE_DEFAULT];
// static gnrc_netreg_entry_t chamos_rserver = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL,
//                                                                KERNEL_PID_UNDEF);

int msgr_process(chamos_msg_t *msg, sock_udp_ep_t *remote) {
    uint8_t iface = get_ieee802154_iface();
    switch (msg->msg_type) {
    case MSG_RS:
        gnrc_ipv6_nib_ft_add(&msg->ip, msg->ip_len, 0, iface, 0);
        printf("Added NIB correctly.\n");
        msg->msg_type = MSG_RS;
        break;
    case MSG_ROFFLINE:
        printf("Deleting NIB entry\n");
        gnrc_ipv6_nib_ft_del(&msg->ip, msg->ip_len);
        msg->msg_type = MSG_ROFFLINE;
        break;
    default:
        printf("Chamos: Error invalid argument");
        return -EINVAL;
    }
    memset(msg, 0, sizeof(chamos_msg_t));
    msg->ip_len = 64;
    get_ipv6_local(iface, &msg->ip);
    sock_udp_send(&chamosr_sock, msg, sizeof(chamos_msg_t), remote);
    return 0;
}

static void *chamosr_thread(void *arg) {
    (void)arg;
    uint8_t chamos_buffer[SERVER_BUFFER_SIZE];
    sock_udp_ep_t remote;
    chamos_msg_t msg;
    while (true) {
        int err_rx = sock_udp_recv(&chamosr_sock, chamos_buffer, sizeof(chamos_buffer),
                                   ROUTING_TIMEOUT_SEQ, &remote);
        if (err_rx <= 0) {
            printf("ERROR %d, FILE: %s, Line %d, function: %s", err_rx, __FILE__, __LINE__,
                   __func__);
            continue;
        }
        if (SERVER_BUFFER_SIZE < sizeof(chamos_msg_t)) {
            printf("Chamos: NIB ADD invalid message size! \n");
            continue;
        }
        memset(&msg, 0, sizeof(chamos_msg_t));
        memcpy(&msg, chamos_buffer, sizeof(chamos_msg_t));

        if (msgr_process(&msg, &remote) < 0) {
            printf("Chamos: Couldn't process message.\n");
        }
    }
    return NULL;
}

int chamos_routing(void) {
    msg_t chamos_msg_queue[CHAMOS_MSG_QUEUE_SIZE];
    sock_udp_ep_t server;
    server.port = CHAMOS_ROUTING_PORT;
    server.family = AF_INET6;
    server.netif = get_ieee802154_iface();
    msg_init_queue(chamos_msg_queue, CHAMOS_MSG_QUEUE_SIZE);
    if (sock_udp_create(&chamosr_sock, &server, NULL, 0) < 0) {
        printf("Couldn't create UDP socket");
        return -1;
    }
    printf("Success: started UDP server on port %u\n", server.port);
    thread_create(chamosr_stack, sizeof(chamosr_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, chamosr_thread, NULL, "chamos_routing");
    return 0;
}
