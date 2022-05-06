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
 * @brief   CHAMOC_client
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
#include "nib.h"

chamos_client_t new_client(char *netif) {
    chamos_client_t client;
    socklen_t client_len = sizeof(client.csock);
    client.socket = socket(PF_INET6, SOCK_DGRAM, 0);
    memset(&client.csock, '\0', sizeof(client.csock));
    client.csock.sin6_family = AF_INET6;
    client.csock.sin6_port = htons(CHAMOC_PORT);
    int8_t err = 0;
    if (inet_pton(AF_INET6, MULTICAST_ADDR, &client.csock.sin6_addr) < 0) {
        printf("Wrong assigning ipv6 multicast address. Error in file %s, in line: %d \n", __FILE__,
               __LINE__);
        err = -1;
    };
    if (setsockopt(client.socket, SOL_SOCKET, SO_BINDTODEVICE, netif, client_len) < 0) {
        printf("Failed binding  Error in file %s, in line: %d \n", __FILE__, __LINE__);
        err = -1;
    }
    if (err < 0) {
        printf("Wrong socket init. Error in file %s, in line: %d \n", __FILE__, __LINE__);
        exit(0);
    }
    return client;
}
