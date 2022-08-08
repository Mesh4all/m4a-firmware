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
 * @ingroup     chamoc CHAMOC Messages
 * @brief       Message structures and functions under CHAMOS, refers all intercomunication, sending
 * and receiving messages from the server
 *
 * @{
 *
 * @file
 *
 * @author      eduazocar <eduardoazocar7@gmail.com>
 */

#ifndef CHAMOC_MSG_H
#define CHAMOC_MSG_H

#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include "client.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief model of an ipv6 address
 *
 */
typedef union {
    uint8_t in_8[16];  /*!< Value ipv6 in bytes e.g: 0x20, 0x01 ...*/
    uint16_t in_16[8]; /*!< Value ipv6 array in each 2 bytes examples: 0x2001, 0x0db8 ...*/
    uint32_t in_32[4]; /*!< Value ipv6 array in each 4 bytes e.g: 0x20010db8, 0x00000001, ...*/
} ipv6_addr_t;

/**
 * @enum chamoc_type_message_t Type of messages presents in CHAMOC and CHAMOS intercomunnication
 * @{
 */
enum chamoc_type_message_t {
    MSG_ACK = 0, /*!< Acknowledge message*/
    MSG_NACK,    /*!< Negative - Acknowledge message*/
    MSG_NIB_ADD, /*!< Neigborh information base add request message*/
    MSG_NIB_DEL, /*!< Neigborh information base del request message*/
};
/**@}*/

/**
 * @brief Struct of a chamoc message, this contains the type of message, sequence number, ipv6
 * address and its prefix.
 *
 */
typedef struct {
    uint8_t message;  /*!< Field to define the type of message {ACK, NACK, NIB_ADD, NIB_DEL}*/
    uint8_t seqno;    /*!< Field to sequence number*/
    uint8_t prefix;   /*!< Saves the ipv6 address length prefix*/
    ipv6_addr_t addr; /*!< Saves the ipv6 address in bytes*/
} chamoc_message_t;

/**
 * @brief  Serializator to CHAMOS message
 * @param[in] msg Contains a chamoc_message_t struct providing [type, seqno, prefix, ipv6 address]
 * @param[out] buffer Output buffer with an byte array containing info about chamoc message
 * @param[in] len size of the buffer
 */
int serialize(chamoc_message_t msg, char *buffer, uint8_t len);

/**
 * @brief  Connect both sockets client and server
 * @param[in] client Chamos Socket client
 * @param[inout] buffer  Can return the last response of the server
 * @param[in] len size of the buffer
 */
int connect_chamos(char* iface, chamos_client_t client, char *buffer, uint8_t len);

/**
 * @brief  increment sequence number of an client;
 * @param[in] client Chamos Socket client
 */
uint8_t craft_seqno(chamos_client_t *client);

/**
 * @brief  send message to the other socket
 * @param[in] client Chamos Socket client
 * @param[in] msg  Process an serialized message and then send it to the server socket
 * @param[in] len size of the buffer
 */
int8_t send_message(chamos_client_t *client, uint8_t *msg, uint8_t len);

#ifdef DEBUG_MODE
void printf_buff(char *buffer, uint8_t len);
#endif

#ifdef __cplusplus
}
#endif

#endif /* CHAMOC_MSG_H */
/** @} */
