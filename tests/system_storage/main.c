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
 * @author      eduazocar <eduazocarv@gmail.com>
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "default_params.h"
#include "embUnit.h"

#include "storage.h"
#define ENABLE_DEBUG 0
#include "debug.h"

#define ADDRESS (uint32_t) flashpage_addr(LAST_AVAILABLE_PAGE - 5)

char data[] = "hello world";
uint8_t u8value = 150;

struct val {
    uint8_t var[6];
    uint32_t var2[6];
    char str[28];
    int32_t var3[50];
};

void test_init_mtd(void) {
    int ret = mtd_start();
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_save_data(void) {
    int ret = 0;
    struct val test_save = {.var = {2, 5, 6, 7, 8, 9},
                            .var2 = {1555, 2556, 477, 8975, 987, 414},
                            .str = "Welcome to mesh storage!!!",
                            .var3 = {1550, 5544, -698, -789, -97852, [49] = 2556}};
    printf("\nSaving data:\n");
    ret = mtd_save_compress(&test_save, sizeof(test_save));
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_load_data(void) {
    struct val test_load;
    printf("\n\nLoading data:");
    DEBUG("\n\n");
    mtd_load(&test_load, sizeof(test_load));
    for (uint16_t i = 0; i < ARRAY_SIZE(test_load.var); i++) {
        DEBUG("varui8_attr1 [%d]: %d\n", i + 1, test_load.var[i]);
    }
    DEBUG("\n");
    for (uint16_t i = 0; i < ARRAY_SIZE(test_load.var2); i++) {
        DEBUG("varui32_attr2 [%d]: %" PRId32 "\n", i + 1, test_load.var2[i]);
    }
    DEBUG("\nvarstr_attr3: %s\n\n", test_load.str);
    for (uint16_t i = 0; i < ARRAY_SIZE(test_load.var3); i++) {
        DEBUG("vari32_attr4 [%d]: %" PRId32 "\n", i + 1, test_load.var3[i]);
    }
    DEBUG("\n");
    printf("\nLoad successfully\n");
}

void test_save_firm_data(void) {
    int ret = 0;
    storage_data_t storage_test = {
        .amount_sensors = 2,
        .wifi_subsys = 0,
        .uniqueid_mode = 0,
        .sensors =
            {
                {
                    .class_sensor = 0,
                    .pin = 5,
                    .sensor_type = 0,
                },
                {
                    .class_sensor = 1,
                    .pin = 1,
                    .sensor_type = 1,
                },
            },
#ifdef MODULE_RADIO
        .radio_tx_power = -30,
        .subghz = true,
        .channel = 11,
#endif
#ifdef MODULE_RPL_PROTOCOL
        .rpl_mode = 0,
        .pan_id = 0x20,
#endif
    };
    ret = mtd_save_compress(&storage_test, sizeof(storage_test));
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_load_firm_data(void) {
    storage_data_t load_storage;
    printf("\n\nLoading Firmware default data:\n\n");
    mtd_load(&load_storage, sizeof(load_storage));
#ifdef MODULE_RADIO
    printf("Tx_power:  %d\n", load_storage.radio_tx_power);
    printf("Sub_GHz:  %s\n", load_storage.subghz ? "YES" : "NO");
    printf("Channel: %u\n", load_storage.channel);
#endif
#ifdef MODULE_RPL_PROTOCOL
    printf("\nRouting\n\nRpl_mode: %s\n", load_storage.rpl_mode ? "DODAG" : "DAG");
    printf("RPL instance: %d\n", load_storage.rpl_instance);
    printf("pan_id: 0x%X\n", load_storage.pan_id);

#endif
    printf("Wifi-subsys: %s\n", load_storage.wifi_subsys ? "YES" : "NO");
    printf("Amount of sensors %d\n", load_storage.amount_sensors);
    for (uint8_t i = 0; i < load_storage.amount_sensors; i++) {
        printf("\nSensor Id %d \n", i + 1);
        printf("Class of Sensor: %s SENSOR\n",
               load_storage.sensors[i].class_sensor ? "TEMPERATURE" : "MOISTURE");
        printf("Application Type of Sensor: %s SENSOR\n",
               load_storage.sensors[i].sensor_type ? "AIR" : "SOIL");
        printf("Input pin: %d\n", load_storage.sensors[i].pin);
    }
}
void test_write_string(void) {
    int ret = mtd_write_string(ADDRESS, data);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_read_string(void) {
    size_t len = mtd_get_string_len(ADDRESS);
    TEST_ASSERT_EQUAL_INT(len, strlen(data) + 1);

    char output[len];
    int ret = mtd_read_string(ADDRESS, output, len);

    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_INT(0, memcmp(data, output, sizeof(output)));
}

void test_write_u8(void) {
    int ret = mtd_write_uint8(ADDRESS, &u8value);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_read_u8(void) {
    uint8_t output = 0;
    int ret = mtd_read_u8(ADDRESS, &output);
    TEST_ASSERT_EQUAL_INT(0, ret);

    TEST_ASSERT_EQUAL_INT(output, u8value);
}

void test_erase_address(void) {
    int ret = mtd_erase_flashpage(ADDRESS);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

Test *tests_mtd_flashpage_tests(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(test_init_mtd),       new_TestFixture(test_save_data),
        new_TestFixture(test_load_data),      new_TestFixture(test_save_firm_data),
        new_TestFixture(test_load_firm_data),
        // new_TestFixture(test_write_string),
        // new_TestFixture(test_read_string),    new_TestFixture(test_write_u8),
        // new_TestFixture(test_read_u8),        new_TestFixture(test_erase_address),
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
