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
 * @ingroup     chamoc
 * @brief       CHAMOC client socket connection
 *
 * @{
 *
 * @file
 *
 * @author      eduazocar <eduardoazocar7@gmail.com>
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

/**
 * @brief MULTICAST ADDRESS is given to start communication to all nodes
 *
 */
#define MULTICAST_ADDR "ff02::1"

/**
 * @brief CHAMOC PORT defined as 6977 to testing, this is not official port
 *
 */
#define CHAMOC_PORT 6977

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Struct Properties or elements of a client
 *
 */
typedef struct {
    uint8_t seqno;             /*!< Sequence number of a message*/
    uint8_t *pending_acks;     /*!< Client pending Aknowledge messages*/
    int socket;                /*!< Socket where it's installed the client*/
    struct sockaddr_in6 csock; /*!< Elements and basic params of clietn socket*/
} chamos_client_t;

/**
 * @brief  Create an socket client under CHAMOS
 * @param[in] netif refers to the name of an interface in the system. example enp2s0
 */
chamos_client_t new_client(char *netif);

#ifdef __cplusplus
}
#endif

#endif /* CLIENT_H */
/** @} */
