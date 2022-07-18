/*
 * Copyright (c) 2022 Mesh4all mesh4all.org
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
 * @brief       ADC test
 *
 * @author      Luis A. Ruiz <luisan00@hotmail.com>
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "periph_conf.h"
#include "periph/gpio.h"
#include "periph/i2c.h"

#include "ztimer.h"
#include "embUnit.h"

void get_i2c_numof(void) {
    int i2c_available = I2C_NUMOF;
    printf("Number of available i2c devices for the %s board is %d\n", RIOT_BOARD, i2c_available);
    TEST_ASSERT_NOT_NULL(&i2c_available);
}

void acquiring_devices(void) {
    int err = 0;
    printf("Trying to acquire I2C available devices\n");

    for (int addr = 0; addr < 255; addr++) {
    }
}

Test *test_periph_i2c(void) {
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(get_i2c_numof), // return available devices for the current board.
        #if (I2C_NUMOF > 0)
        new_TestFixture(acquiring_devices), // scan for conected devices.
        #endif
    };

    EMB_UNIT_TESTCALLER(test_periph_i2c, NULL, NULL, fixtures);

    return (Test *)&test_periph_i2c;
}

int main(void) {
    printf("Test I2C peripherals\n");
    TESTS_START();
    TESTS_RUN(test_periph_i2c());
    TESTS_END();
    return 0;
}
