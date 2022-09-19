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

ipv6_addr_t addr;

void test_get_ipv6_static(void){
    get_uid_ipv6(&addr, UNIQUEID_STATIC_MODE);
    ipv6_addr_print(&addr);
    printf("\n");
}

void test_get_ipv6_random(void){
    get_uid_ipv6(&addr, UNIQUEID_RANDOM_MODE);
    ipv6_addr_print(&addr);
    printf("\n");
}

void test_uid_random_blocks(void){
    random_generator(64);
}

Test *tests_get_unique_id(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(test_get_ipv6_static),
        new_TestFixture(test_get_ipv6_random),
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
