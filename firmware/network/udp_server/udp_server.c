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
 * @brief  udp libs
 *
 * @author  xkevin190 <kevinvelasco190@gmail.com>
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "thread.h"
#include "udp_server.h"

#define SERVER_MSG_QUEUE_SIZE (8)
#define SERVER_BUFFER_SIZE (100)

bool server_running = false;
sock_udp_t sock;
uint8_t server_buffer[SERVER_BUFFER_SIZE];
char server_stack[THREAD_STACKSIZE_DEFAULT];
msg_t server_msg_queue[SERVER_MSG_QUEUE_SIZE];

void *_udp_serverf(void *args) {
    udpf_payload *castPayload = (udpf_payload *)args;
    sock_udp_ep_t server = {.port = atoi(castPayload->port), .family = AF_INET6};
    msg_init_queue(server_msg_queue, SERVER_MSG_QUEUE_SIZE);

    if (sock_udp_create(&sock, &server, NULL, 0) < 0) {
        return NULL;
    }

    server_running = true;
    printf("Success: started UDP server on port %u\n", server.port);
    while (1) {
        int res =
            sock_udp_recv(&sock, server_buffer, sizeof(server_buffer) - 1, SOCK_NO_TIMEOUT, NULL);
        if (res > 0) {
            castPayload->callback(server_buffer, res);
        }
    }

    return NULL;
}

int udp_server(udpf_payload *argv) {
    if ((server_running == false) &&
        thread_create(server_stack, sizeof(server_stack), THREAD_PRIORITY_MAIN - 1,
                      THREAD_CREATE_STACKTEST, _udp_serverf, argv,
                      "UDP Server") <= KERNEL_PID_UNDEF) {
        return -1;
    }

    return 0;
}
