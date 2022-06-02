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
 * @brief  Border Radio.
 *
 * @author  RocioRojas <rociorojas391@gmail.com>
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

int border_router_add_ipv6(int cast_type, ipv6_addr_t *addr, uint8_t iface_type) {
    uint16_t flags = GNRC_NETIF_IPV6_ADDRS_FLAGS_STATE_VALID;
    uint8_t prefix_len = _IPV6_DEFAULT_PREFIX_LEN;
    netif_t *iface = NULL;
    ipv6_addr_t ip;
    int8_t index;
    if (iface_type == WIRED_INTERFACE) {
        index = get_wired_iface();
    } else if (iface_type == WIRELESS_INTERFACE) {
        index = get_ieee802154_iface();
    } else {
        printf("Error: Type of Interface doesn't exists File: %s, line: %d\n", __FILE__, __LINE__);
        return -1;
    }
    if (index == -1) {
        printf("Error: Expected interface wasn't found. File: %s, line %d\n", __FILE__, __LINE__);
        return -1;
    }
    if (get_ipv6_global(index, &ip) == 0) {
        printf("Error: Already exists an ipv6 Address File: %s, line: %d\n", __FILE__, __LINE__);
        return -1;
    }
    iface = netif_get_by_id(index);
    if (cast_type == _ANYCAST || cast_type == _UNICAST || cast_type == _MULTICAST) {
        if (cast_type == _MULTICAST) {
            if (ipv6_addr_is_multicast(addr)) {
                if (netif_set_opt(iface, NETOPT_IPV6_GROUP, 0, addr, sizeof(ipv6_addr_t)) < 0) {
                    printf("error: unable to join IPv6 multicast group\n");
                    return -1;
                }
            } else {
                return -1;
            }
        } else {
            if (cast_type == _ANYCAST) {
                flags |= GNRC_NETIF_IPV6_ADDRS_FLAGS_ANYCAST;
            }
            flags |= (prefix_len << 8U);
            if (netif_set_opt(iface, NETOPT_IPV6_ADDR, flags, addr, sizeof(ipv6_addr_t)) < 0) {
                printf("error: unable to add IPv6 address\n");
                return -1;
            }
        }
    } else {
        return -1;
    }
    return 0;
}
