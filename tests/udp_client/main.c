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
 * @brief       udp client test
 *
 * @author      xkevin190 <kevinvelasco193@gmail.com>
 */
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "embUnit.h"
#include "udp_client.h"
#include "udp_server.h"

int port = 3000;

void udp_callback(uint8_t *message, size_t len) {
    (void)len;
    printf("message is %s \n", (char *)message);
}

static void setup(void) {

    udpf_payload payload;
    payload.port = "3000";
    payload.callback = &udp_callback;

    udp_server(&payload);
}

void send_udp_message_to_ipv6(void) {
    char message[] = "hello world";
    size_t len = strlen(message) + 1;
    int err = udp_send(port, "::1", (uint8_t *)message, &len);
    TEST_ASSERT_EQUAL_INT(0, err);
}

Test *tests_udp_client(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(send_udp_message_to_ipv6),
    };

    EMB_UNIT_TESTCALLER(tests_udp_client, NULL, NULL, fixtures);

    return (Test *)&tests_udp_client;
}

int main(void) {
    setup();
    TESTS_START();
    TESTS_RUN(tests_udp_client());
    TESTS_END();
    return 0;
}
