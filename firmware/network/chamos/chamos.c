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

#include "net_tools.h"
#include "chamos.h"
#include "net/gnrc/ipv6/nib/ft.h"
#include "xtimer.h"

#define CHAMOS_MSG_QUEUE_SIZE (8)
#define SERVER_BUFFER_SIZE (CONFIG_SERVER_BUFFER_SIZE)

gnrc_netif_t *_netif;
sock_udp_t chamos_sock = {};
char chamos_stack[THREAD_STACKSIZE_DEFAULT];

int server_send_ack(chamos_msg_t *msg, sock_udp_ep_t *remote, bool flag_ack_val) {
    if (flag_ack_val) {
        msg->msg_type = MSG_ACK;
    } else {
        msg->msg_type = MSG_NACK;
    }
    int8_t iface = get_wired_iface();
    memset(&msg->ip, 0, sizeof(ipv6_addr_t));
    get_ipv6_local(iface, &msg->ip);
    return sock_udp_send(&chamos_sock, msg, sizeof(chamos_msg_t), remote);
}

int msg_process(chamos_msg_t *msg) {
    int res;
    switch (msg->msg_type) {
    case MSG_NIB_ADD:
        printf("Adding NIB entry\n");
        res = gnrc_ipv6_nib_ft_add(&msg->ip, msg->ip_len, 0, _netif->pid, 0);
        if (res == -EINVAL) {
            printf("Chamos: Error invalid argument.\n");
            return -EINVAL;
        } else if (res == -ENOMEM) {
            printf("Chamos: Error Out of memory.\n");
            return -ENOMEM;
        } else if (res == -ENOTSUP) {
            printf("Chamos: Error Operation not supported on transport endpoint.\n");
            return -ENOTSUP;
        } else {
            printf("Added NIB correctly.\n");
        }
        break;
    case MSG_NIB_DEL:
        printf("Deleting NIB entry\n");
        gnrc_ipv6_nib_ft_del(&msg->ip, msg->ip_len);
        break;

    default:
        printf("Chamos: Error invalid argument");
        return -EINVAL;
    }
    return 0;
}

int chamos_parse_buff(chamos_msg_t *chamos_msg, uint8_t *buffer, size_t len) {
    if (len < sizeof(chamos_msg_t)) {
        printf("Chamos: NIB ADD invalid message size! \n");
        return -EINVAL;
    }
    memset(chamos_msg, 0, sizeof(chamos_msg_t));
    memcpy(chamos_msg, buffer, sizeof(chamos_msg_t));
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
                printf("Chamos: address is already used \n");
            }
            if (err_rx == -EAFNOSUPPORT) {
                printf("Chamos: endpoint is not supported.\n");
            }
            if (err_rx == -EINVAL) {
                printf(
                    "Chamos: remote has an invalid address or does not have a valid interface.\n");
            }
            if (err_rx == -ENOMEM) {
                printf("Chamos: not enough resources can be provided for `chamos_sock` to be "
                       "created \n");
            }
            continue;
        }
        if (err_rx == 0) {
            printf("Chamos: packet doesn't have a payload, dropping \n");
            continue;
        }
        if (chamos_parse_buff(&msg, chamos_buffer, err_rx) < 0) {
            printf("Chamos: Couldn't parse received message.\n");
        } else {
            flag_ack = true;
            if (msg_process(&msg) < 0) {
                printf("Chamos: Couldn't process message.\n");
                flag_ack = false;
            }
        }
        if (server_send_ack(&msg, &remote, flag_ack) < 0) {
            printf("Chamos: Couldn't send the ACK!\n");
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
        printf("Invalid IPv6 group address");
        return -EINVAL;
    }
    if (gnrc_netif_ipv6_group_join(netiface, &group) < 0) {
        printf("Couldn't join IPv6 group");
        return -1;
    }
    if (sock_udp_create(&chamos_sock, &server, NULL, 0) < 0) {
        printf("Couldn't create UDP socket");
        return -1;
    }
    memcpy(&server.addr, &group, sizeof(ipv6_addr_t));
    _netif = netiface;
    printf("Success: started UDP server on port %u\n", server.port);
    thread_create(chamos_stack, sizeof(chamos_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, chamos_thread, NULL, "chamos");
    return 0;
}
