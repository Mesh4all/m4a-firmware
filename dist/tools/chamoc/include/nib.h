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
 * @ingroup     CHAMOC CHAMOC NIB
 * @brief       Provide modes to generate an nib_add or nib_del request and send it to the server.
 *
 * @{
 *
 * @file
 *
 * @author      eduazocar <eduardoazocar7@gmail.com>
 */
#ifndef NIB_H
#define NIB_H

#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Function gives the necessary settings to provide an socket a nib add or nib_del address
 * request.
 * @param[in] Iface refers to the name of an interface in the system. example enp2s0
 * @param[in] prefix the prefix of the Ipv6 address
 * @param[in] ip ipv6 address represented by a string
 * @param[in] msg_type Represent the message type (NIB_ADD or NIB_DEL)
 */
int nib_req(char *iface, char *ip, uint8_t prefix, uint8_t msg_type);

#ifdef __cplusplus
}
#endif

#endif /* NIB_H */
/** @} */
