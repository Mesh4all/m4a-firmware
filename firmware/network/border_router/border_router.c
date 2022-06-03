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
 * @brief   Border Router Module
 *
 * @author  RocioRojas <rociorojas391@gmail.com>
 * @author  eduazocar <eduazocarv@gmail.com>
 */
#include <stdio.h>
#include <string.h>
#include "border_router.h"
#include "radio.h"
#include "net_tools.h"

int8_t get_wired_iface(void) {
    int max_ifaces = gnrc_netif_numof();
    if (max_ifaces > 0) {
        gnrc_netif_t *iface;
        iface = gnrc_netif_get_by_type(NETDEV_ANY, NETDEV_INDEX_ANY);
        if (iface != NULL) {
            return iface->pid;
        } else {
            return -1;
        }
    }
    return -1;
}

int8_t border_router_setup(ipv6_addr_t addr, uint8_t iface_type) {
    ipv6_addr_t ip;
    int8_t index;
    switch (iface_type) {
    case WIRED_INTERFACE:
        index = get_wired_iface();
        break;
    case WIRELESS_INTERFACE:
        index = get_ieee802154_iface();
        break;
    default:
        printf("Error: Type of Interface doesn't exists File: %s, line: %d\n", __FILE__, __LINE__);
        return -1;
    }
    if (index < 0) {
        printf("Error: Expected interface wasn't found. File: %s, line %d\n", __FILE__, __LINE__);
        return -1;
    }
    if (get_ipv6_global(index, &ip) == 0) {
        printf("Error: Already exists an ipv6 Address File: %s, line: %d\n", __FILE__, __LINE__);
        return -1;
    }
    if (ipv6_addr_is_global(&addr)) {
        if (set_ipv6_global(index, addr) < 0) {
            return -1;
        }
        return 0;
    }
    if (ipv6_addr_is_multicast(&addr)) {
        if (set_ipv6_multicast(index, addr) < 0) {
            return -1;
        }
        return 0;
    }
    printf("Error: Only can be processed Unicast and Multicast Addresses. File: %s, line: %d\n",
           __FILE__, __LINE__);
    return -1;
}
