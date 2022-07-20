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
 * @brief       Radio file
 *
 * @author      RocioRojas <rociorojas391@gmail.com>
 */
#include <string.h>
#include <errno.h>
#include "embUnit.h"
#include "radio.h"

void test_get_ieee802154_iface(void) {
    int err = get_ieee802154_iface()>0 ? 0:-1;
    TEST_ASSERT_EQUAL_INT(0, err);
}

void test_get_netopt_tx_power(void) {
    int16_t txpower=0;
    int8_t err =  get_netopt_tx_power(txpower);
    TEST_ASSERT_EQUAL_INT(0, err);
}

void test_get_netopt_channel(void) {
    int16_t channel=0;
    int8_t err = get_netopt_channel(channel);
    TEST_ASSERT_EQUAL_INT(0, err);
}

void test_initial_radio_setup(void) {
    int8_t err = initial_radio_setup();
    TEST_ASSERT_EQUAL_INT(0, err);
}

Test *tests_radio_module(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(test_get_ieee802154_iface),
        new_TestFixture(test_get_netopt_tx_power),
        new_TestFixture(test_get_netopt_channel),
        new_TestFixture(test_initial_radio_setup),
    };

    EMB_UNIT_TESTCALLER(radio_tests, NULL, NULL, fixtures);

    return (Test *)&radio_tests;
}

int main(void) {
    TESTS_START();
    TESTS_RUN(tests_radio_module());
    TESTS_END();
    return 0;
}
