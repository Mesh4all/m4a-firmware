/*
 * Copyright (C) 2022 Mesh4all.org <mesh4all.org>
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/**
 * @ingroup     net_tools
 * @{
 *
 * @file
 * @brief       Group of generic functions that could be used in various modules implementation
 *
 * @author      eduazocar <eduardoazocar7@gmail.com>
 * @author      RocioRojas <Rociorojas391@gmail.com>
 *
 * @}
 */

#include <stdint.h>
#include <stdio.h>
#include "net_tools.h"
#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"

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

int8_t get_ipv6_global(kernel_pid_t iface_pid, ipv6_addr_t *addr) {
    ipv6_addr_t ipv6[CONFIG_GNRC_NETIF_IPV6_ADDRS_NUMOF];
    netif_t *iface = netif_get_by_id(iface_pid);
    int8_t num_of_addr = netif_get_opt(iface, NETOPT_IPV6_ADDR, 0, ipv6,
                                       CONFIG_GNRC_NETIF_IPV6_ADDRS_NUMOF * sizeof(ipv6_addr_t));
    if (num_of_addr < 0) {
        printf("Error: Not ipv6 address found. File: %s,  Function: %s, Line: %d\n", __FILE__,
               __func__, __LINE__);
        return -1;
    }
    for (unsigned i = 0; i < (num_of_addr / sizeof(ipv6_addr_t)); i++) {
        if (ipv6_addr_is_global(&ipv6[i])) {
            *addr = ipv6[i];
            return 0;
        }
    }
    return -1;
}

int8_t set_ipv6_global(kernel_pid_t iface_index, ipv6_addr_t ip, uint8_t prefix) {
    if (prefix > 128) {
        printf("Wrong prefix length\n"
               "File: %s, Function: %s, Line: %d\n",
               __FILE__, __func__, __LINE__);
    }
    uint16_t flags = GNRC_NETIF_IPV6_ADDRS_FLAGS_STATE_VALID |
(prefix << CONFIG_PREFIX_DISPLACEMENT);
    netif_t *iface = netif_get_by_id(iface_index);
    if (!ipv6_addr_is_global(&ip)) {
        printf("The ipv6 address isn't a ipv6 global address format accepted\n"
               "File: %s, Function: %s, Line: %d\n",
               __FILE__, __func__, __LINE__);
        return -1;
    }
    if (netif_set_opt(iface, NETOPT_IPV6_ADDR, flags, &ip,
sizeof(ipv6_addr_t)) < CONFIG_PREFIX_DISPLACEMENT) {
        printf("error: unable to add IPv6 address\n"
               "File: %s, Function: %s, Line: %d\n",
               __FILE__, __func__, __LINE__);
        return -1;
    }
    return 0;
}

int8_t set_ipv6_multicast(kernel_pid_t iface_index, ipv6_addr_t ip, uint8_t prefix) {
    if (prefix > 128) {
        printf("Wrong prefix length\n"
               "File: %s, Function: %s, Line: %d\n",
               __FILE__, __func__, __LINE__);
    }
    uint16_t flags = GNRC_NETIF_IPV6_ADDRS_FLAGS_STATE_VALID |
(prefix << CONFIG_PREFIX_DISPLACEMENT);
    netif_t *iface = netif_get_by_id(iface_index);
    if (!ipv6_addr_is_multicast(&ip)) {
        printf("The ipv6 address isn't a ipv6 multicast address format accepted\n"
               "File: %s, Function: %s, Line: %d\n",
               __FILE__, __func__, __LINE__);
        return -1;
    }
    if (netif_set_opt(iface, NETOPT_IPV6_GROUP, flags, &ip, sizeof(ipv6_addr_t)) < 0) {
        printf("error: unable to join IPv6 multicast group\n"
               "File: %s, Function: %s, Line: %d\n",
               __FILE__, __func__, __LINE__);
        return -1;
    }
    return 0;
}
