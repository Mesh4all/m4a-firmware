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
#include "storage_register.h"
#include "storage_internal.h"

#include "mtd.h"

#if (CONFIG_DEBUG_TEST_STORAGE) || (DOXYGEN)
/**
 * @brief KCONFIG_PARAMETER TO SET DEBUG MODE
 *
 */
#define ENABLE_DEBUG CONFIG_DEBUG_TEST_STORAGE
#else
#define ENABLE_DEBUG 0
#endif

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
    mtd_dump();
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
    mtd_clear_all_regs();
    int ret = 0;
    char str[28] = {"Everyone in the mesh!!!"};
    char str2[20] = {"Contributor: CW"};
    uint8_t age = 24;
    uint16_t port = 4094;
    uint32_t u32_val = 155556;
    int8_t i8val = -123;
    int16_t i16val = -3258;
    int32_t i32val = -9000;
    uint8_t index, numreg;
    ret = mtd_put_str(str, "KEY0", sizeof(str));
    if (ret < 0) {
        DEBUG("Failed Saving data");
    }
    printf("\n");
    ret = mtd_put_str(str2, "KEY1", sizeof(str2));
    if (ret < 0) {
        DEBUG("Failed Saving data\n");
    }
    ret = mtd_put_u8(age, "KEY2");
    if (ret < 0) {
        DEBUG("Failed Saving data\n");
    }
    ret = mtd_put_u16(port, "KEY3");
    if (ret < 0) {
        DEBUG("Failed Saving data\n");
    }
    ret = mtd_put_u32(u32_val, "KEY4");
    if (ret < 0) {
        DEBUG("Failed Saving data\n");
    }
    ret = mtd_put_i8(i8val, "KEY5");
    if (ret < 0) {
        DEBUG("Failed Saving data\n");
    }
    ret = mtd_put_i16(i16val, "KEY6");
    if (ret < 0) {
        DEBUG("Failed Saving data\n");
    }
    ret = mtd_put_i32(i32val, "KEY7");
    if (ret < 0) {
        DEBUG("Failed Saving data\n");
    }
    mtd_available_idx(&index, &numreg);
    TEST_ASSERT_EQUAL_INT(0, ret);
    mtd_dump();
}

void test_loading_reg(void) {
    int ret = 0;
    char str[28];
    char str2[20];
    uint8_t age;
    uint16_t port;
    uint32_t u32_val;
    int8_t i8val;
    int16_t i16val;
    int32_t i32val;
    ret = mtd_get_str(str, "KEY0", sizeof(str));
    if (ret < 0) {
        DEBUG("Failed Loading data");
    }
    ret = mtd_load_reg(str2, "KEY1", sizeof(str2));
    if (ret < 0) {
        DEBUG("Failed Loading data");
    }
    ret = mtd_get_u8(&age, "KEY2");
    if (ret < 0) {
        DEBUG("Failed Loading data");
    }
    ret = mtd_get_u16(&port, "KEY3");
    if (ret < 0) {
        DEBUG("Failed Loading data");
    }
    ret = mtd_get_u32(&u32_val, "KEY4");
    if (ret < 0) {
        DEBUG("Failed Loading data");
    }
    ret = mtd_get_i8(&i8val, "KEY5");
    if (ret < 0) {
        DEBUG("Failed Loading data");
    }
    ret = mtd_get_i16(&i16val, "KEY6");
    if (ret < 0) {
        DEBUG("Failed Loading data");
    }
    ret = mtd_get_i32(&i32val, "KEY7");
    mtd_reg_del("KEY2", sizeof(uint8_t));
    printf("string#1 loaded: %s\n", str);
    printf("string#2 loaded: %s\n", str2);
    printf("uint8_t loaded: %d\n", age);
    printf("uint16_t loaded: %" PRIu16 "\n", port);
    printf("uint32_t loaded: %" PRIu32 "\n", u32_val);
    printf("int8_t loaded: %d\n", i8val);
    printf("int16_t loaded: %" PRId16 " \n", i16val);
    printf("int32_t loaded: %" PRId32 " \n", i32val);
    mtd_dump();
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
