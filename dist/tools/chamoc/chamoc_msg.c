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
 * @brief   CHAMOC_MSG
 *
 * @author  eduazocar<eduazocarv@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client.h"
#include "chamoc_msg.h"

int serialize(chamoc_message_t msg, char *buffer, uint8_t len) {
    uint8_t strbuff[sizeof(chamoc_message_t)] = {msg.message, msg.seqno, msg.prefix};
    for (int i = 0; i < sizeof(ipv6_addr_t); i++) {
        strbuff[3 + i] = msg.addr.in_8[i];
    }
    memcpy(buffer, strbuff, len);

    return 1;
}

int connect_chamos(char *iface, chamos_client_t client, char *buffer, uint8_t len) {
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    int tries = 0;
    if (setsockopt(client.socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) < 0) {
        printf("configuration_failed\n");
        return -1;
    }

    while (tries < 5) {
        send_message(&client, (uint8_t *)buffer, len);
        recv(client.socket, buffer, len, 0);
        if (buffer[0] == MSG_ACK) {
            char link_ipv6[40] = {0};
            printf("Request Confirmed\n");
            printf("Confirmed from %s\n",
                   inet_ntop(AF_INET6, &buffer[3], link_ipv6, sizeof(client.csock)));
            char cmd[120];
            sprintf(cmd, "sudo ip route a ::/0 via %s dev %s", link_ipv6, iface);
            system(cmd);
            break;
        } else {
            tries++;
            if (tries < 5) {
                printf("Bad Request, Trying Again\n");
            } else {
                printf("Time Out\n\n");
            }
        }
    }
    return 1;
}

uint8_t craft_seqno(chamos_client_t *client) {
    client->seqno += 1;
    return client->seqno;
}

int8_t send_message(chamos_client_t *client, uint8_t *msg, uint8_t len) {
    if (client->socket < 0) {
        return -1;
    }
    sendto(client->socket, msg, len, 0, (struct sockaddr *)&client->csock, sizeof(client->csock));
    return 1;
}
