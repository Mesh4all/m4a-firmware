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
 * @ingroup     Chamos
 * @{
 * @file
 * @brief       CHAMOS Communication Handler for Addressing Management | Origin Server
 * @author      RocioRojas <Rociorojas391@gmail.com>
 *
 */

#ifndef CHAMOS_H
#define CHAMOS_H

#include "net/gnrc.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum  chamos_msg_types_t Types of messages manage in the chamos server
 * @{
 */
enum chamos_msg_types_t {
    MSG_ACK = 0,     /*!< Message acknowledged */
    MSG_NACK = 1,    /*!< Message not acknowledged */
    MSG_NIB_ADD = 2, /*!< Add entry to NIB */
    MSG_NIB_DEL = 3, /*!< Delete entry from NIB */
};
/**@}*/

/**
 * @brief   CHAMOS message
 */
typedef struct {
    uint8_t msg_type; /*!< Message type */
    uint8_t seqno;    /*!< Sequence number */
    uint8_t ip_len;   /*!< IP address pfx_len */
    ipv6_addr_t ip;   /*!< IP address */
} chamos_msg_t;

/**
 * @brief this function is used to send ack
 *
 * @param [in] msg            Frame
 * @param [in] remote         sock udp
 * @param [in] flag_ack_val  bool
 * @return int
 */
int server_send_ack(chamos_msg_t *msg, sock_udp_ep_t *remote, bool flag_ack_val);

/**
 * @brief this function is used to init the module
 *
 * @param  [in]  netiface
 * @param  [in]  port
 * @return int
 */
int chamos_init(uint16_t port, gnrc_netif_t *netiface);

#ifdef __cplusplus
}
#endif
#endif /* CHAMOS_H */
/** @} */
