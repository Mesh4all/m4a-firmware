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
 * @ingroup     network
 * @file        border_router.h
 * @brief       this module content all functions of Border Router
 * @author      RocioRojas <rociorojas391@gmail.com>
 *
 */

#ifndef BORDER_ROUTER_H
#define BORDER_ROUTER_H

#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief   The default IPv6 prefix length if not specified.
 */
#define _IPV6_DEFAULT_PREFIX_LEN (64U)

/**
 * @brief List of cast types to the ipv6 address
 *
 */
enum cast_t {
       _UNICAST = 0,   /*!< Sets a process by which a packet is sent
                         from one host to an individual host*/
       _ANYCAST,       /*!< Sets a method forwards messages to a single
                         device of a specific group of devices. */
       _MULTICAST      /*!< Sets multicasting addresses messages for a
                         specific group of devices in a network */
       };

/**
 * @brief This function it's set to border router to host.
 *
 * @param [in] cast_type cast_type you want to set
 * @param [in] addr       ipv6 address
 * @return int
 */
int border_router_add_ipv6(int cast_type, ipv6_addr_t *addr);

/**
 * @brief This function it's set to border router to node.
 *
 * @param[in] cast_type cast_type you want to set
 * @param[in] addr      ipv6 address
 * @return int
 */
int border_router_add_ipv6_node(int cast_type, ipv6_addr_t *addr);

#ifdef __cplusplus
}
#endif
#endif /* BORDER_ROUTER_H */
       /**@}*/
