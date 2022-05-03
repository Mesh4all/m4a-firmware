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
 * @brief       border router test file
 *
 * @author      RocioRojas <Rociorojas391@gmail.com>
 */

#include <string.h>
#include <errno.h>
#include "embUnit.h"
#include "border_router.h"
#include "rpl_protocol.h"

void test_border_router_add_ipv6(void) {
     ipv6_addr_t address = {
        .u8 = {0},
    };
    ipv6_addr_from_str(&address, "2001:db8:1::2");
    int err = border_router_add_ipv6(_UNICAST,  &address);
    TEST_ASSERT_EQUAL_INT(0, err);
}

void test_border_router_add_ipv6_node(void) {
     ipv6_addr_t address = {
        .u8 = {0},
    };
    ipv6_addr_from_str(&address, "2001:db8:1::2");
    int err = border_router_add_ipv6_node(_UNICAST,  &address);
    TEST_ASSERT_EQUAL_INT(0, err);
}

Test *tests_border_router(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(test_border_router_add_ipv6),
        new_TestFixture(test_border_router_add_ipv6_node),
    };

    EMB_UNIT_TESTCALLER(tests_border_router, NULL, NULL, fixtures);

    return (Test *)&tests_border_router;
}

int main(void) {
    TESTS_START();
    TESTS_RUN(tests_border_router());
    TESTS_END();
    return 0;
}
