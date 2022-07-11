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
 * @brief       udp server test
 *
 * @author      xkevin190 <kevinvelasco193@gmail.com>
 */
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "embUnit.h"
#include "udp_client.h"
#include "udp_server.h"
#include "xtimer.h"

int port = CONFIG_UDP_SERVER_PORT;

void udp_callback(uint8_t *message, size_t len) {
    (void)len;
    (void)message;
    printf("%s\n", message);
}

static void start_udp(void) {

    udpf_payload payload;
    payload.port = CONFIG_UDP_SERVER_PORT;
    payload.callback = &udp_callback;

    int err = udp_server(&payload);
    TEST_ASSERT_EQUAL_INT(0, err);
}

void send_udp_message_to_ipv6(void) {
    char message[] = "hello world";
    size_t len = strlen(message) + 1;
    int err = udp_send(port, "::1", (uint8_t *)message, &len);
    TEST_ASSERT_EQUAL_INT(0, err);
}

int main(void) {

    start_udp();
    while (true) {
        xtimer_sleep(2);
        send_udp_message_to_ipv6();
    }
    return 0;
}
