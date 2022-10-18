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
 * @brief       rpl test file
 *
 * @author      xkevin190 <kevinvelasco193@gmail.com>
 */
#include <string.h>
#include <errno.h>

#include "embUnit.h"
#include "uniqueid.h"
#include "rpl_protocol.h"
#include "net_tools.h"
#include "radio.h"

#define IFACE_ID 4
#define DODAG_INSTANCE 1

void test_init_rpl(void) {
    int err = rpl_init(IFACE_ID);
    TEST_ASSERT_EQUAL_INT(0, err);
}

void test_add_dodag_node(void) {
    ipv6_addr_t ipv6 = {
        .u8 = {0x20, 0x01, 0x0d, 0xb8, 0x00, 0x02, 0x00, 0x01},
    };
    uint8_t iface_index = get_ieee802154_iface();
    set_ipv6_by_uid(iface_index, &ipv6, 64, UNIQUEID_STATIC_MODE);
    int err = gnrc_rpl_dodag_root(DODAG_INSTANCE, &ipv6);
    TEST_ASSERT_EQUAL_INT(0, err);
}

void test_node_state_dodag(void) {
    int8_t rpl_mode;
    int8_t rpl_inst;
    rpl_inst = get_instance_id();
    rpl_mode = get_rpl_mode_state();
    printf("Node state %s, Instance Id %d\n", rpl_mode < 1? "DAG" : "DODAG", rpl_inst);
    TEST_ASSERT_EQUAL_INT(DODAG, rpl_mode);
}

void test_remove_dodag_node(void) {
    int err = rpl_dodag_remove(DODAG_INSTANCE);
    TEST_ASSERT_EQUAL_INT(0, err);
}

void test_node_state_dag(void) {
    int8_t rpl_mode;
    int8_t rpl_inst;
    rpl_inst = get_instance_id();
    rpl_mode = get_rpl_mode_state();
    printf("Node state %s, Instance Id %d\n", rpl_mode < 1? "DAG" : "DODAG", rpl_inst);
    TEST_ASSERT_EQUAL_INT(DAG, rpl_mode);
}

Test *tests_rpl_protocol(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(test_init_rpl),
        new_TestFixture(test_add_dodag_node),
        new_TestFixture(test_node_state_dodag),
        new_TestFixture(test_remove_dodag_node),
        new_TestFixture(test_node_state_dag),
    };

    EMB_UNIT_TESTCALLER(tests_rpl_protocol, NULL, NULL, fixtures);

    return (Test *)&tests_rpl_protocol;
}

int main(void) {
    TESTS_START();
    TESTS_RUN(tests_rpl_protocol());
    TESTS_END();
    return 0;
}
