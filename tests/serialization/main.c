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
 * @brief       storage file
 *
 * @author      xkevin190 <kevinvelasco193@gmail.com>
 */
#include <string.h>
#include <errno.h>

#include "embUnit.h"
#include "serialization.h"

sensor_data payload = {
    .soil_moisture = 200,
    .temperature = 50,
};

size_t len = 0;

uint8_t buffer[64];

void test_enconde_message(void) {
    int err = cbor_enconde_message(&payload, buffer, &len);
    TEST_ASSERT_EQUAL_INT(0, err);
}

void test_decode_message(void) {
    sensor_data output;

    cbor_decode_message(buffer, &output, len);

    TEST_ASSERT_EQUAL_INT(payload.temperature, output.temperature);
    TEST_ASSERT_EQUAL_INT(payload.soil_moisture, output.soil_moisture);
}

Test *tests_mtd_flashpage_tests(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(test_enconde_message),
        new_TestFixture(test_decode_message),
    };

    EMB_UNIT_TESTCALLER(mtd_flashpage_tests, NULL, NULL, fixtures);

    return (Test *)&mtd_flashpage_tests;
}

int main(void) {
    TESTS_START();
    TESTS_RUN(tests_mtd_flashpage_tests());
    TESTS_END();
    return 0;
}
