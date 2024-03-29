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
 * @defgroup    net_tools net_tools
 * @ingroup     network_net_tools
 * @brief       group of generic functions that could be used in various modules
 *
 * @{
 *
 * @file
 *
 * @author      eduazocar <eduardoazocar7@gmail.com>
 */

#ifndef NET_TOOLS_H
#define NET_TOOLS_H

/* Add header includes here */
#include <stdio.h>
#include <stdint.h>
#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BITS_IN_A_BYTE (8)
#define PREFIX_LSHIFTTED_BITS (8)

/**
 * @brief Function to check if exist a wired interface
 *
 * @retval returns the index of the network interface.
 * @retval -1 when the interface doesn't exist
 */
int8_t get_wired_iface(void);

/**
 * @brief Function to check if exist an ipv6 global unicast
 *
 * @param[in] iface_pid it's the id of an interface
 * @param[out] addr variable ipv6 where will be saved the found unicast global address
 *
 * @retval 0 if exist an ipv6 address and saves the located address in @p addr
 * @retval -1 when the interface doesn't have an unicast global address
 */
int8_t get_ipv6_global(kernel_pid_t iface_pid, ipv6_addr_t *addr);

/**
 * @brief Function to set an ipv6 global address in an iface.
 *
 * @param[in] iface_index  index that will be save the ipv6 address.
 * @param[in] ip Address ipv6 global to set.
 * @param[in] prefix Networks prefix represent the subnet size.
 * @retval 0 Correctly set up of the @p ip in the interface.
 * @retval -1 Couldn't set the @p ip address in the interface.
 */
int8_t set_ipv6_global(kernel_pid_t iface_index, ipv6_addr_t ip, uint8_t prefix);
/**
 * @brief Remove the current global ipv6 in an interface.
 *
 * @param iface_idx interface index to removes the global ipv6 address.
 * @retval 0 address removed successful.
 * @retval -1 Doesn't exist a global ipv6 address.
 */
int8_t rm_ipv6_global(kernel_pid_t iface_idx);
/**
 * @brief Function to set an ipv6 global address in an iface.
 *
 * @param[in] iface_index  index that will be save the ipv6 address.
 * @param[in] ip Address ipv6 multicast to set
 * @param[in] prefix Networks prefix represent the subnet size.
 * @retval  0 Correctly set up of the @p ip in the interface.
 * @retval -1 Couldn't set the @p ip address in the interface.
 */
int8_t set_ipv6_multicast(kernel_pid_t iface_index, ipv6_addr_t ip, uint8_t prefix);

#if (MODULE_UNIQUEID) || (DOXYGEN)
/**
 * @brief Function to set an ipv6 global address in an iface.
 *
 * @param[in] iface_index  index that will be save the ipv6 address.
 * @param[inout] ip Address ipv6 to set. the octets of the required IPv6 address should be
 * previously defined (depending on the network prefixes)
 * @param[in] prefix Networks prefix represent the subnet size.
 * @param uid_mode define if the uniqueid generate random or static ipv6 address
 * @retval  0 Correctly set up of the @p ip in the interface.
 * @retval -1 Couldn't set the @p ip address in the interface.
 */
int8_t set_ipv6_by_uid(const kernel_pid_t iface_index, ipv6_addr_t *ip, const uint8_t prefix,
                       const uint8_t uid_mode);
#endif
#ifdef __cplusplus
}
#endif

#endif /* NET_TOOLS_H */
/** @} */
