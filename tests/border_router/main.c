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
 * @author      eduazocar <eduazocarv@gmail.com>
 */

#include <string.h>
#include <errno.h>
#include "embUnit.h"
#include "border_router.h"

void test_border_router_setup_wired(void) {
    ipv6_addr_t address = {
        .u8 = {0},
    };
    ipv6_addr_from_str(&address, CONFIG_ADDRESS_IPV6_WIRED);
    int err = border_router_setup(address, 64, CONFIG_WIRED_INTERFACE);
    TEST_ASSERT_EQUAL_INT(0, err);
}
void test_border_router_setup_wireless(void) {
    ipv6_addr_t address = {
        .u8 = {0},
    };
    ipv6_addr_from_str(&address, CONFIG_ADDRESS_IPV6_WIRELESS);
    int err = border_router_setup(address, 16, CONFIG_WIRELESS_INTERFACE);
    TEST_ASSERT_EQUAL_INT(0, err);
}

Test *tests_border_router(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(test_border_router_setup_wired),
        new_TestFixture(test_border_router_setup_wireless),
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
