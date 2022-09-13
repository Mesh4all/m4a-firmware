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
 * @brief   CHAMOS  Communication Handler for Addressing Management | Origin Server
 *
 * @author  RocioRojas <Rociorojas391@gmail.com>
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "chamos.h"
#include "net/gnrc/ipv6/nib/ft.h"
#include "xtimer.h"

#if (CONFIG_DEBUG_CHAMOS) || (DOXYGEN)
/**
 * @brief KCONFIG_PARAMETER TO SET DEBUG MODE
 *
 */
#define ENABLE_DEBUG CONFIG_DEBUG_CHAMOS
#else
#define ENABLE_DEBUG 0
#endif

#include "debug.h"

#define CHAMOS_MSG_QUEUE_SIZE (8)
#define SERVER_BUFFER_SIZE (CONFIG_SERVER_BUFFER_SIZE)

gnrc_netif_t *_netif;
sock_udp_t chamos_sock = {};
char chamos_stack[THREAD_STACKSIZE_DEFAULT];

int server_send_ack(chamos_msg_t *msg, sock_udp_ep_t *remote, bool flag_ack_val) {
    uint8_t buf[2];
    if (flag_ack_val) {
        buf[0] = MSG_ACK;
    } else {
        buf[0] = MSG_NACK;
    }
    buf[1] = msg->seqno;
    return sock_udp_send(&chamos_sock, buf, sizeof(buf), remote);
}

int msg_process(chamos_msg_t *msg) {
    int res;
    switch (msg->msg_type) {
    case MSG_NIB_ADD:
        DEBUG("Adding NIB entry\n");
        ipv6_addr_t next_hop = {.u8 = {0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}};
        res = gnrc_ipv6_nib_ft_add(&msg->ip, msg->ip_len, &next_hop, _netif->pid, 0);
        if (res == -EINVAL) {
            DEBUG("Chamos: Error invalid argument.\n");
            return -EINVAL;
        } else if (res == -ENOMEM) {
            DEBUG("Chamos: Error Out of memory.\n");
            return -ENOMEM;
        } else if (res == -ENOTSUP) {
            DEBUG("Chamos: Error Operation not supported on transport endpoint.\n");
            return -ENOTSUP;
        } else {
            DEBUG("Added NIB correctly.\n");
        }
        break;
    case MSG_NIB_DEL:
        DEBUG("Deleting NIB entry\n");
        gnrc_ipv6_nib_ft_del(&msg->ip, msg->ip_len);
        break;

    default:
        DEBUG("Chamos: Error invalid argument");
        return -EINVAL;
    }
    return 0;
}

int chamos_parse_buff(chamos_msg_t *chamos_msg, uint8_t *buffer, size_t len) {
    memset(chamos_msg, 0, sizeof(chamos_msg_t));
    if (len < 3) {
        DEBUG("Chamos: invalid message size! \n");
        return -EINVAL;
    }
    uint8_t type = buffer[0];
    uint8_t seqno = buffer[1];
    uint8_t ip_len = buffer[2];
    switch (type) {
    case MSG_NIB_ADD:
        if (len < sizeof(chamos_msg_t)) {
            DEBUG("Chamos: NIB ADD invalid message size! \n");
            return -EINVAL;
        }
        chamos_msg->msg_type = type;
        chamos_msg->seqno = seqno;
        chamos_msg->ip_len = ip_len;
        if (memcmp(&buffer[3], &chamos_msg->ip, sizeof(ipv6_addr_t)) == 0) {
            DEBUG("Chamos: Invalid IPV6 address.\n");
            return -EINVAL;
        }
        memcpy(&chamos_msg->ip, &buffer[3], sizeof(ipv6_addr_t));
        break;
    case MSG_NIB_DEL:
        if (len < sizeof(chamos_msg_t)) {
            DEBUG("Chamos: NIB DEL invalid message size! \n");
            return -EINVAL;
        }
        chamos_msg->msg_type = type;
        chamos_msg->seqno = seqno;
        chamos_msg->ip_len = ip_len;
        if (memcmp(&buffer[3], &chamos_msg->ip, sizeof(ipv6_addr_t)) == 0) {
            DEBUG("Chamos: Invalid IPV6 address.\n");
            return -EINVAL;
        }
        memcpy(&chamos_msg->ip, &buffer[3], sizeof(ipv6_addr_t));
        break;
    default:
        DEBUG("Chamos: Invalid message type \n");
        return -EINVAL;
    }
    return 0;
}

void *chamos_thread(void *arg) {
    (void)arg;
    uint8_t chamos_buffer[SERVER_BUFFER_SIZE];
    sock_udp_ep_t remote;
    chamos_msg_t msg;
    bool flag_ack = false;
    while (true) {
        int err_rx = sock_udp_recv(&chamos_sock, chamos_buffer, sizeof(chamos_buffer),
                                   SOCK_NO_TIMEOUT, &remote);
        if (err_rx < 0) {
            if (err_rx == -EADDRINUSE) {
                DEBUG("Chamos: address is already used \n");
            }
            if (err_rx == -EAFNOSUPPORT) {
                DEBUG("Chamos: endpoint is not supported.\n");
            }
            if (err_rx == -EINVAL) {
                DEBUG(
                    "Chamos: remote has an invalid address or does not have a valid interface.\n");
            }
            if (err_rx == -ENOMEM) {
                DEBUG("Chamos: not enough resources can be provided for `chamos_sock` to be "
                      "created \n");
            }
            continue;
        }
        if (err_rx == 0) {
            DEBUG("Chamos: packet doesn't have a payload, dropping \n");
            continue;
        }
        if (chamos_parse_buff(&msg, chamos_buffer, err_rx) < 0) {
            DEBUG("Chamos: Couldn't parse received message.\n");
        } else {
            flag_ack = true;
            if (msg_process(&msg) < 0) {
                DEBUG("Chamos: Couldn't process message.\n");
                flag_ack = false;
            }
        }
        if (server_send_ack(&msg, &remote, flag_ack) < 0) {
            DEBUG("Chamos: Couldn't send the ACK!\n");
        }
    }
    return NULL;
}

int chamos_init(uint16_t port, gnrc_netif_t *netiface) {
    msg_t chamos_msg_queue[CHAMOS_MSG_QUEUE_SIZE];
    sock_udp_ep_t server;
    server.port = port;
    server.family = AF_INET6;
    server.netif = netiface->pid;
    ipv6_addr_t group;
    msg_init_queue(chamos_msg_queue, CHAMOS_MSG_QUEUE_SIZE);
    if (ipv6_addr_from_str(&group, CONFIG_HEADER_ADDRESS_MULTICAST) == NULL) {
        DEBUG("Invalid IPv6 group address");
        return -EINVAL;
    }
    if (gnrc_netif_ipv6_group_join(netiface, &group) < 0) {
        DEBUG("Couldn't join IPv6 group");
        return -1;
    }
    if (sock_udp_create(&chamos_sock, &server, NULL, 0) < 0) {
        DEBUG("Couldn't create UDP socket");
        return -1;
    }
    memcpy(&server.addr, &group, sizeof(ipv6_addr_t));
    _netif = netiface;
    DEBUG("Success: started UDP server on port %u\n", server.port);
    thread_create(chamos_stack, sizeof(chamos_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, chamos_thread, NULL, "chamos");
    return 0;
}
