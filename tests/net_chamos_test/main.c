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
 * @brief       chamos test
 *
 * @author      RocioRojas <RocioRojas391@gmail.com>
 */
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "embUnit.h"
#include "chamos.h"
#include "border_router.h"
#define SERVER_BUFFER_SIZE (100)

uint8_t chamos_buffer_test[SERVER_BUFFER_SIZE];

void start_chamos(void) {

    gnrc_netif_t *iface;
    iface = gnrc_netif_get_by_type(NETDEV_ANY, NETDEV_INDEX_ANY);
    if (iface == NULL) {
        return;
    }
    int err = chamos_init(1337, iface);
    TEST_ASSERT_EQUAL_INT(0, err);
}

void send_udp_message_to_chamos(void) {
    chamos_msg_t msg;
    msg.msg_type = MSG_NIB_ADD;
    msg.seqno = 0;
    msg.ip_len = 64;
    ipv6_addr_from_str(&msg.ip, "2001:db8:");
    uint8_t buf[2];
    ssize_t res;
sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
   local.port = 1338;
     sock_udp_ep_t remote = {.family = AF_INET6};

    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, "::1") == NULL) {
        puts("Error: unable to parse destination address");
        return;
    }
    if (ipv6_addr_is_link_local((ipv6_addr_t *)&remote.addr)) {
        gnrc_netif_t *netif = gnrc_netif_get_by_type(NETDEV_ANY, NETDEV_INDEX_ANY);
        remote.netif = (uint16_t)netif->pid;
    }
    remote.port = 1337;

    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP sock.\n");
        return;
    }

    if (sock_udp_send(&sock, &msg, sizeof(msg), &remote) < 0) {
        puts("Error sending message.\n");
        sock_udp_close(&sock);
        return;
    }
    if ((res = sock_udp_recv(&sock, buf, sizeof(buf), 1 * US_PER_SEC, NULL)) < 0) {
        if (res == -ETIMEDOUT) {
            puts("Timed out.\n");
        } else {
            puts("Error receiving message.\n");
        }
    } else {
        printf("Received message: \"");
        for (int i = 0; i < res; i++) {
            printf("%x", buf[i]);
        }
        printf("\"\n");
    }
    if (buf[0] == MSG_ACK) {
        printf("Received ACK \n");
    } else {
        printf("Received NACK \n");
    }
    sock_udp_close(&sock);
    TEST_ASSERT_EQUAL_INT(MSG_ACK, buf[0]);
}

Test *tests_chamos_tests(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(start_chamos),
        new_TestFixture(send_udp_message_to_chamos),
    };

    EMB_UNIT_TESTCALLER(chamos_tests, NULL, NULL, fixtures);

    return (Test *)&chamos_tests;
}

int main(void) {
    TESTS_START();
    TESTS_RUN(tests_chamos_tests());
    TESTS_END();
    return 0;
}
