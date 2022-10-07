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
 * @ingroup     tests
 * @{
 *
 * @file
 * @brief       test to net_tools functions
 *
 * @author      eduazocar <eduardoazocar7@gmail.com>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "embUnit.h"
#include "net_tools.h"
#include "uniqueid.h"

ipv6_addr_t addr;
char saddr[40];

void test_set_ipv6_uid_static(void) {
    uint8_t iface = get_wired_iface();
    rm_ipv6_global(iface);
    ipv6_addr_from_str(&addr, "2001:db8:2:1::");
    set_ipv6_by_uid(iface, &addr, 64, UNIQUEID_STATIC_MODE);
    ipv6_addr_print(&addr);
    printf("\n");
    ipv6_addr_to_str(saddr, &addr, sizeof(saddr));
    TEST_ASSERT(strcmp(saddr, "2001:db8:2:1::") != 0);
}

void test_set_ipv6_via_uid_rnd(void) {
    uint8_t iface = get_wired_iface();
    ipv6_addr_from_str(&addr, "2001:db8:2:1::");
    set_ipv6_by_uid(iface, &addr, 64, UNIQUEID_RANDOM_MODE);
    ipv6_addr_print(&addr);
    printf("\n");
    ipv6_addr_to_str(saddr, &addr, sizeof(saddr));
    TEST_ASSERT(strcmp(saddr, "2001:db8:2:1::") != 0);
}

void test_get_ipv6_addr(void) {
    ipv6_addr_t ip;
    int err = 0;
    uint8_t iface = get_wired_iface();
    err = get_ipv6_global(iface, &ip);
    TEST_ASSERT_EQUAL_INT(0, err);
}

void test_rm_global_addr(void) {
    int err = 0;
    uint8_t iface = get_wired_iface();
    err = rm_ipv6_global(iface);
    TEST_ASSERT_EQUAL_INT(0, err);
}

test_set_ipv6(void) {
    ipv6_addr_t ip;
    int err = 0;
    uint8_t iface = get_wired_iface();
    err = set_ipv6_global(iface, ip, 64);
    TEST_ASSERT_EQUAL_INT(0, err);
}

Test *tests_net_tools(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(test_set_ipv6_uid_static), new_TestFixture(test_set_ipv6_via_uid_rnd),
        new_TestFixture(test_get_ipv6_addr),       new_TestFixture(test_set_ipv6),
        new_TestFixture(test_rm_global_addr),

    };

    EMB_UNIT_TESTCALLER(tests_net_tools, NULL, NULL, fixtures);

    return (Test *)&tests_net_tools;
}

int main(void) {
    TESTS_START();
    TESTS_RUN(tests_net_tools());
    TESTS_END();
    return 0;
}
