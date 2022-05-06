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
 * @brief   chamoc_nib
 *
 * @author  eduazocar<eduazocarv@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "client.h"
#include "chamoc_msg.h"
#include "nib.h"

int nib_req(char *iface, char *ip, uint8_t prefix, uint8_t msg_type) {
    if (msg_type == MSG_NIB_ADD || msg_type == MSG_NIB_DEL) {
        chamos_client_t client = new_client(iface);
        char buff[sizeof(chamoc_message_t)];
        chamoc_message_t message = {
            .message = msg_type,
            .prefix = prefix,
            .seqno = 0,
        };
        if (inet_pton(AF_INET6, ip, &message.addr) < 0) {
            printf("Wrong parsing ipv6 address to message strutc, Error in File: %s, Inline: %d",
                   __FILE__, __LINE__);
            return -1;
        };
        serialize(message, buff, sizeof(buff));
        connect_chamos(client, buff, sizeof(buff));
        close(client.socket);
        return 1;
    } else {
        printf("Message type not supported in nib request\nError in file: %s in line %d\n",
               __FILE__, __LINE__);
        return -1;
    }

    return 1;
}
