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
 * @brief       chamoc_server_test
 *
 * @author      eduazocar <eduazocarv@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "chamoc_msg.h"

void main(int argc, char **argv) {
    if (argc < 2 || argc > 3 || argv[2] == NULL) {
        printf("Usage: %s <iface> <port>\n", argv[0]);
        exit(0);
    } else {
    }
    char *iface = argv[1];
    int port = atoi(argv[2]);
    int sockfd;
    struct sockaddr_in6 si_me, si_other;
    char buffer[19];
    socklen_t addr_size;

    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);

    int reuseaddr;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, iface, strlen(iface)) < 0) {
        printf("failed init server binding to device\n Error in file: %s, in line: %d\n", __FILE__,
               __LINE__);
        exit(0);
    }
    memset(&si_me, '\0', sizeof(si_me));
    si_me.sin6_family = AF_INET6;
    si_me.sin6_port = htons(port);
    si_me.sin6_addr = in6addr_any;
    bind(sockfd, (struct sockaddr *)&si_me, sizeof(si_me));
    addr_size = sizeof(si_other);
    memset(buffer, 0, sizeof(buffer));
    printf("Server it's running\n");
    recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&si_other, &addr_size);
    printf("Packet Received: ");
    for (int i = 0; i < sizeof(buffer); i++) {
        printf("%02hhx", (uint8_t)buffer[i]);
    }
    if (buffer[0] == MSG_NIB_ADD || MSG_NIB_DEL) {
        buffer[0] = MSG_ACK;
    } else {
        buffer[0] = MSG_NACK;
    }
    printf("\n");
    sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&si_other, addr_size);
}
