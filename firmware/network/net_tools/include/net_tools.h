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
 * @defgroup    net_tools
 * @ingroup     network
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

/**
 * @brief Function to check if exist an ipv6 global unicast
 *
 * @param[in] iface_pid it's the id of an interface
 * @param[out] addr variable ipv6 where will be saved the found unicast global address
 *
 * @returns 0 if exist an ipv6 address and saves the located address in @p addr
 * @returns -1 when the interface doesn't have an unicast global address
 */
int8_t get_ipv6_global(kernel_pid_t iface_pid, ipv6_addr_t *addr);

#ifdef __cplusplus
}
#endif

#endif /* NET_TOOLS_H */
/** @} */
