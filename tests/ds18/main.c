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
#include "ds18_sensor.h"

void test_init_ds18(void) {
    int err = init_temperature_sensor(CONFIG_PIN_TEMP_SENSOR);

    TEST_ASSERT_EQUAL_INT(0, err);
}

void test_ds18_get_temperature(void) {
    int16_t output = 0;
    int err = get_temperature_value(&output);

    TEST_ASSERT_EQUAL_INT(0, err);
}

Test *tests_ds18b20_sensor(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){new_TestFixture(test_init_ds18),
                                    new_TestFixture(test_ds18_get_temperature)};

    EMB_UNIT_TESTCALLER(ds18b20_tests, NULL, NULL, fixtures);

    return (Test *)&ds18b20_tests;
}

int main(void) {
    TESTS_START();
    TESTS_RUN(tests_ds18b20_sensor());
    TESTS_END();
    return 0;
}
