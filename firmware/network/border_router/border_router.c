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

#if (CONFIG_DEBUG_BORDER_ROUTER) || (DOXYGEN)
/**
 * @brief KCONFIG_PARAMETER TO SET DEBUG MODE
 *
 */
#define ENABLE_DEBUG CONFIG_DEBUG_BORDER_ROUTER
#endif

#ifndef ENABLE_DEBUG
#define ENABLE_DEBUG 0
#endif

#include "debug.h"

int8_t border_router_setup(ipv6_addr_t addr, uint8_t prefix, uint8_t iface_type) {
    ipv6_addr_t ip;
    int8_t index;

    switch (iface_type) {
    case CONFIG_WIRED_INTERFACE:
        index = get_wired_iface();
        break;
    case CONFIG_WIRELESS_INTERFACE:
        index = get_ieee802154_iface();
        break;
    default:
        DEBUG("Error: Type of Interface doesn't exists File: %s, line: %d\n", __FILE__, __LINE__);
        return -1;
    }

    if (index < 0) {
        DEBUG("Error: Expected interface wasn't found. File: %s, line %d\n", __FILE__, __LINE__);
        return -1;
    }
    if (get_ipv6_global(index, &ip) == 0) {
        DEBUG("Error: Already exists an ipv6 Address File: %s, line: %d\n", __FILE__, __LINE__);
        return -1;
    }
    if (ipv6_addr_is_global(&addr)) {
        if (set_ipv6_global(index, addr, prefix) < 0) {
            return -1;
        }
        return 0;
    }
    if (ipv6_addr_is_multicast(&addr)) {
        if (set_ipv6_multicast(index, addr, prefix) < 0) {
            return -1;
        }
        return 0;
    }

    DEBUG("Error: Only can be processed Unicast and Multicast Addresses. File: %s, line: %d\n",
           __FILE__, __LINE__);
    return -1;
}
