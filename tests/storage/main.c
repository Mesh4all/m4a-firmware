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

#define ADDRESS (uint32_t) flashpage_addr(LAST_AVAILABLE_PAGE - 5)

char data[] = "hello world";
uint8_t u8value = 150;

struct val {
    uint8_t var[6];
    uint32_t var2[6];
    char str[28];
    int32_t var3[5];
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
                            .var3 = {1550, 5544, -698, -789, -97852}};
    printf("\nSaving data:\n");
    mtd_save_compress(&test_save, sizeof(test_save));
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_load_data(void) {
    struct val test_load;
    printf("\n\nLoading data:\n\n");
    mtd_load(&test_load, sizeof(test_load));
    for (uint16_t i = 0; i < ARRAY_SIZE(test_load.var); i++) {
        printf("varui8_attr1 [%d]: %d\n", i + 1, test_load.var[i]);
    }
    printf("\n");
    for (uint16_t i = 0; i < ARRAY_SIZE(test_load.var2); i++) {
        printf("varui32_attr2 [%d]: %" PRId32 "\n", i + 1, test_load.var2[i]);
    }
    printf("\n");
    printf("varstr_attr3: %s\n\n", test_load.str);
    for (uint16_t i = 0; i < ARRAY_SIZE(test_load.var3); i++) {
        printf("vari32_attr4 [%d]: %" PRId32 "\n", i + 1, test_load.var3[i]);
    }
    printf("\n");
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
        new_TestFixture(test_init_mtd),    new_TestFixture(test_save_data),
        new_TestFixture(test_load_data),   new_TestFixture(test_write_string),
        new_TestFixture(test_read_string), new_TestFixture(test_write_u8),
        new_TestFixture(test_read_u8),     new_TestFixture(test_erase_address),
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
