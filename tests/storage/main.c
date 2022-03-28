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

#include "storage.h"

#define ADDRESS (uint32_t) flashpage_addr(LAST_AVAILABLE_PAGE -5)

char data[] = "hello world";
uint8_t u8value = 150;

void test_init_mtd(void) {
    int ret = mtd_start();
    TEST_ASSERT_EQUAL_INT(0, ret);
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
        new_TestFixture(test_init_mtd),    new_TestFixture(test_write_string),
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
