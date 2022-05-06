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
 * @defgroup    brnim Border Router Neighborh Information Manager
 * @ingroup     network
 * @brief       System manager to get information of the all neighbors connected to border router
 *
 * @{
 *
 * @file
 *
 * @author      eduazocar <eduardoazocar7@gmail.com>
 * @author      RocioRojas <rociorojas391@gmail.com>
 */

#ifndef BRNIM_H
#define BRNIM_H


#define MULTICAST_ROOT_NODE_IPV6_ADDRESS "ff02::1"

enum {
    MSG_ACK = 0,  /**< Message acknowledged */
    MSG_NACK = 1, /**< Message not acknowledged */
    MSG_NIB_ADD = 2
    MSG_NIB_DEL = 3,  /**< Delete entry from NIB */
}
/* Add header includes here */

typedef struct {
    uint8_t msg; /**< Message type */
    uint8_t seqno; /**< Sequence number */
    ipv6_addr_t address;
    uint8_t prefix-len;
} brnim_adv;

#ifdef __cplusplus
extern "C" {
#endif

/* Declare the API of the module */

#ifdef __cplusplus
}
#endif

#endif /* BRNIM_H */
/** @} */
