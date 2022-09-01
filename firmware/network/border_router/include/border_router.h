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
 * @{
 * @ingroup     border_router
 * @file        border_router.h
 * @brief       this module content all functions of Border Router
 * @author      RocioRojas <rociorojas391@gmail.com>
 * @author      eduazocar <eduazocarv@gmail.com>
 */

#ifndef BORDER_ROUTER_H
#define BORDER_ROUTER_H

#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BR_ROUTING_PORT_SERVER (7000)
#define BR_ROUTING_PORT_CLIENT (6999)
#define ROUTING_TIMEOUT_SEQ (1 * 60000000) // check the routes per minute //

enum br_msg_types_t { MSG_RS = 199 };

typedef struct {
    uint8_t type;   /*!< Message type */
    uint8_t seqno;  /*!< Sequence number */
    uint8_t prefix; /*!< IP address pfx_len */
    ipv6_addr_t ip; /*!< IP address */
} br_msg_t;

typedef struct __attribute__((packed)) {
    ipv6_addr_t dest_route;
    uint8_t prefix;
} br_radv_t;

/**
 * @brief This function init the border router. sets ipv6 in an interface.
 *
 * @param[in] addr       ipv6 address.
 * @param[in] iface_type refers to if is used a WIRED or WIRELESS interface.
 * @param[in] prefix     Networks prefix,express the subnet size.
 * @retval 0 Setup Success
 * @retval -1 Setup Failed
 */
int8_t border_router_setup(ipv6_addr_t addr, uint8_t prefix, uint8_t iface_type);

/**
 * @brief RIS (Router intercomunnication system) Initialize
 *
 * @param[in] addr       ipv6 address.
 * @param[in] iface_type refers to if is used a WIRED or WIRELESS interface.
 * @param[in] prefix     Networks prefix,express the subnet size.
 * @retval 0 Setup Success
 * @retval -1 Setup Failed
 */
int8_t init_br_routing(void);
#ifdef __cplusplus
}
#endif
#endif /* BORDER_ROUTER_H */
       /**@}*/
