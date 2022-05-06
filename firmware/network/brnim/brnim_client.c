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
 * @ingroup     module_brnim
 * @{
 *
 * @file
 * @brief       Border Router Neighborh Information Manager implementation
 *
 * @author      eduazocar <eduardoazocar7@gmail.com>
 * @author      RocioRojas <rociorojas391@gmail.com>
 *
 * @}
 */

#ifdef MODULE_BRNIM_CLIENT
#include "brnim.h"
#include "net/sock/udp.h"
#include "net/gnrc/ipv6/nib/ft.h"

static sock_udp_t _sock;

/**
 * @brief   GNRC netif
 */
static gnrc_netif_t *_netif;

/**
 * @brief   UDP local endpoint
 */
static sock_udp_ep_t _local;

#endif
/* Implementation of the module */
