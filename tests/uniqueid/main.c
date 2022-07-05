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
 * @brief       uniqueid file
 *
 * @author  xkevin190 <kevinvelasco193@gmail.com>
 * @author  RocioRojas <rociorojas391@gmail.com>
 * @author  eduazocar <eduazocarv@gmail.com>
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "embUnit.h"
#include "uniqueid.h"
#include "unique_random.h"

void get_unique_from_mac(ipv6_addr_t *output) {
    ipv6_addr_t header = {
        .u8 = {0},
    };
    char addr_cpu[CPUID_LEN] = {0};
    CPUID(addr_cpu);
    ipv6_addr_from_str(&header, CONFIG_HEADER_ADDRESS_ID);
    memcpy((char *)output->u8, (char *)header.u8, 4);
    strncat((char *)output->u8, addr_cpu, 4);
    ipv6_addr_print(output);
}

void test_get_ipv6Address(void) {
    ipv6_addr_t ipv6 = {
        .u8 = {0},
    };

    subnet_to_ipv6(&ipv6);
#ifdef CONFIG_MODE_STATIC
    ipv6_addr_t output = {
        .u8 = {0},
    };
    ipv6_addr_print(&ipv6);
    get_unique_from_mac(&output);
    TEST_ASSERT_EQUAL_INT(1, ipv6_addr_equal(&ipv6, &output));
#endif

#ifdef CONFIG_MODE_RANDOM
    ipv6_addr_t output1 = {
        .u8 = {0},
    };
    ipv6_addr_t output2 = {
        .u8 = {0},
    };

    subnet_to_ipv6(&output1);
    subnet_to_ipv6(&output2);
    printf("\nFirst random IPv6\n");
    ipv6_addr_print(&output1);
    printf("\nSecond random IPv6\n");
    ipv6_addr_print(&output2);
    TEST_ASSERT_EQUAL_INT(0, ipv6_addr_equal(&output1, &output2));
    printf("\n\nBinary data printed\n");
    random_generator(64);
#endif
}

Test *tests_get_unique_id(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(test_get_ipv6Address),
    };

    EMB_UNIT_TESTCALLER(tests_get_unique_id, NULL, NULL, fixtures);

    return (Test *)&tests_get_unique_id;
}

int main(void) {
    TESTS_START();
    TESTS_RUN(tests_get_unique_id());
    TESTS_END();
    return 0;
}
