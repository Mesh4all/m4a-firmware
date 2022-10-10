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
#include "embUnit.h"

#include "storage.h"
#include "mtd.h"
#define ENABLE_DEBUG 0
#include "debug.h"

struct val {
    uint8_t var[8];
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
    mtd_erase_all();
    struct val test_save = {
        .var = {2, 5, 150, 7, 33, 9},
        .var2 = {1555, 2556, 477, 8975, 987, 454},
        .str = "Welcome to mesh storage!!!",
        .var3 = {1550, 5544, -698, -789, -97852, [19] = -25, [48] = 39, [49] = 2556}};
    printf("\nSaving data:\n");
    ret = mtd_save(&test_save, sizeof(test_save), MTD_START_ADDR);
    if (ret < 0) {
        DEBUG("Failed Saving data\n");
    }
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_load_data(void) {
    struct val test_load;
    printf("\n\nLoading data:");
    DEBUG("\n\n");
    mtd_load(&test_load, sizeof(test_load), MTD_START_ADDR);
    for (uint16_t i = 0; i < ARRAY_SIZE(test_load.var); i++) {
        DEBUG("varui8_attr1 [%d]: %d\n", i, test_load.var[i]);
    }
    DEBUG("\n");
    for (uint16_t i = 0; i < ARRAY_SIZE(test_load.var2); i++) {
        DEBUG("varui32_attr2 [%d]: %" PRId32 "\n", i, test_load.var2[i]);
    }
    DEBUG("\nvarstr_attr3: %s\n\n", test_load.str);
    for (uint16_t i = 0; i < ARRAY_SIZE(test_load.var3); i++) {
        DEBUG("vari32_attr4 [%d]: %" PRId32 "\n", i, test_load.var3[i]);
    }
    DEBUG("\n");
    printf("\nLoad successfully\n");
}

void test_save_pointed(void) {
    int ret = 0;
    mtd_erase_all();
    char str[36] = {"mesh4all, connect with everyone!!!"};
    ret = mtd_save(str, sizeof(str), MTD_START_ADDR + 56);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_load_pointed(void) {
    int ret = 0;
    char str[36];
    ret = mtd_load(str, sizeof(str), MTD_START_ADDR + 56);
    DEBUG("\nvarstr_attr3: %s\n\n", str);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_saving_reg(void) {
    int ret = 0;
    mtd_erase_all();
    char str[28] = {"Everyone in the mesh!!!"};
    char str2[20] = {"Contributor: CW"};
    uint8_t age = 24;
    uint32_t u32_val = 155556;

    ret = mtd_save_reg(str, (uint8_t *)"KEY0", sizeof(str));
    if (ret < 0) {
        DEBUG("Failed Saving data");
    }
    printf("\n");
    ret = mtd_save_reg(str2, (uint8_t *)"KEY1", sizeof(str2));
    if (ret < 0) {
        DEBUG("Failed Saving data\n");
    }
    ret = mtd_save_reg(str, (uint8_t *)"KEY0", sizeof(str));
    if (ret < 0) {
        DEBUG("Failed Saving data\n");
    }
    ret = mtd_save_reg(&age, (uint8_t *)"KEY2", sizeof(age));
    if (ret < 0) {
        DEBUG("Failed Saving data\n");
    }
    ret = mtd_save_reg(&u32_val, (uint8_t *)"KEY3", sizeof(u32_val));
    mtd_available_idx();
    TEST_ASSERT_EQUAL_INT(0, ret);
    mtd_dump();
}

void test_loading_reg(void) {
    int ret = 0;
    char str[28];
    char str2[20];
    uint32_t u32_val;
    uint8_t age;
    ret = mtd_load_reg(str, (uint8_t *)"KEY0", sizeof(str));
    if (ret < 0) {
        DEBUG("Failed Loading data");
    }
    ret = mtd_load_reg(&u32_val, (uint8_t *)"KEY3", sizeof(u32_val));
    if (ret < 0) {
        DEBUG("Failed Loading data");
    }
    ret = mtd_load_reg(str2, (uint8_t *)"KEY1", sizeof(str2));
    if (ret < 0) {
        DEBUG("Failed Loading data");
    }
    ret = mtd_load_reg(&age, (uint8_t *)"KEY2", sizeof(age));

    printf("String#1 loaded: %s\n", str);
    printf("String#2 loaded: %s\n", str2);
    printf("Uint32_t loaded: %" PRIu32 "\n", u32_val);
    printf("Uint8_t loaded: %d\n", age);

    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_erase_all(void) {
    int ret = mtd_erase_all();
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_load_post_erased_data(void) {
    int ret = mtd_dump();
    TEST_ASSERT_EQUAL_INT(-1, ret);
}

Test *tests_mtd_flashpage_tests(void) {
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(test_init_mtd),
        new_TestFixture(test_saving_reg),
        new_TestFixture(test_loading_reg),
        new_TestFixture(test_save_data),
        new_TestFixture(test_load_data),
        new_TestFixture(test_save_pointed),
        new_TestFixture(test_load_pointed),
        new_TestFixture(test_erase_all),
        new_TestFixture(test_load_post_erased_data),
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
