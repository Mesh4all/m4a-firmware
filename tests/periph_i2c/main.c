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
 * @ingroup     test_group_periph_i2c
 * @{
 * @file
 * @brief       I2C low level test
 *
 * @author      Luis A. Ruiz <luisan00@hotmail.com>
 * @}
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "periph_conf.h"
#include "periph/gpio.h"
#include "periph/i2c.h"

#include "embUnit.h"

uint16_t i2c_scan_dev(i2c_t dev) {
    i2c_acquire(dev);
    uint16_t ack_addr = 0;
    for (char i = 0; i < 8; i++) {
        uint16_t addr = i;
        addr <<= 4;
        for (unsigned j = 0; j < 16; j++) {

            char dummy[1];
            int retval;
            while (-EAGAIN == (retval = i2c_read_byte(dev, addr, dummy, 0))) {
            }
            switch (retval) {
            case 0:
                ack_addr = addr;
                break;
            case -ENXIO:
                break;
            default:
                printf("I2C_DEV(%d) - Error: 0x%x\n", dev, addr);
                break;
            }
            addr++;
        }
    }
    i2c_release(dev);
    return ack_addr;
}

void get_i2c_numof(void) {
    printf("Number of available i2c ports in the %s board: %d\n", RIOT_BOARD, I2C_NUMOF);
    TEST_ASSERT(I2C_NUMOF > 0);
}

void scan_devices(void) {

    int dev_found = 0;

    for (size_t idx = 0; idx < I2C_NUMOF; idx++) {
        uint16_t addr = i2c_scan_dev(I2C_DEV(idx));
        printf("I2C_DEV(%d) Found: 0x%x\n", (int)idx, addr);
        if (addr) {
            dev_found = 1;
        }
    }
    if (!dev_found) {
        printf("No devices\n");
    }

    TEST_ASSERT_EQUAL_INT(1, dev_found);
}

Test *test_periph_i2c(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(get_i2c_numof), // Return available i2c ports for the current board.
        new_TestFixture(scan_devices),  // Scan for connected devices on the available ports.
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
