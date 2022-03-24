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
#include "udp_client.h"

int udp_send(int *port, char *address, uint8_t *message, size_t *payload_len) {
    int res;
    sock_udp_ep_t remote = {.family = AF_INET6};

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
    if ((res = sock_udp_send(NULL, message, *payload_len, &remote)) < 0) {
        puts("could not send");
    } else {
        printf("Success: send %u bytes  from %s \n", (unsigned)res, address);
    }
    return 0;
}
