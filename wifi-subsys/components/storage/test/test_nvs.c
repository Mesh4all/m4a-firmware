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
 * @brief   NVS (Non Volatile Storage) test
 *
 * @author  luisan00 <luisan00@hotmail.com>
 */

#include "default_params.h"
#include "esp_log.h"
#include "storage.h"
#include "unity.h"

/* uint8_t default params */
static const char *u8_key = "u8_key";
static uint8_t u8_value = 255U;

/* uint32_t default params */
static const char *u32_key = "u32_key";
static uint32_t u32_value = 4294967295U;

/* c string  default params */
static const char *str_key = "str_key";
static const char *str_value = "str_default_value";

/* also we need to test some related macro helper, this should be changed */
#define TEST

TEST_CASE("Literal to String", "nvs") {
    const char *expected = "TEST";
    ESP_LOGI(stringlify(TEST), "stringlify: %s ?= expected: %s", stringlify(TEST), expected);

    if (strcmp(expected, stringlify(TEST)) != 0) {
        TEST_FAIL();
    }
}

TEST_CASE("Initialize the default nvs partition", "nvs") {
    esp_err_t err = nvs_init();
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("Save a uint8_t in the NVS", "nvs") {
    esp_err_t err = nvs_set_uint8(stringlify(TEST), u8_key, u8_value);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("Get a previusly saved uint8_t from the nvs", "nvs") {
    uint8_t value = 0U;

    esp_err_t err = nvs_get_uint8(stringlify(TEST), u8_key, &value);

    ESP_LOGI(stringlify(TEST), "%d ?= %d", u8_value, value);

    if (err != ESP_OK) {
        TEST_FAIL();
    }

    if (u8_value != value) {
        TEST_FAIL();
    }
}

TEST_CASE("Save a uint32_t in the NVS", "nvs") {
    esp_err_t err = nvs_set_uint32(stringlify(TEST), u32_key, u32_value);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("Get a previusly saved uint32_t from the nvs", "nvs") {
    uint32_t value = 0U;

    esp_err_t err = nvs_get_uint32(stringlify(TEST), u32_key, &value);

    ESP_LOGI(stringlify(TEST), "%d ?= %d", u32_value, value);

    if (err != ESP_OK) {
        TEST_FAIL();
    }

    if (u32_value != value) {
        TEST_FAIL();
    }
}

TEST_CASE("Save a c string in the nvs", "nvs") {
    esp_err_t err = nvs_set_string(stringlify(TEST), str_key, str_value);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("Get a previously saved c string from the nvs", "nvs") {
    char *buff = NULL;
    size_t buff_size = 0;

    esp_err_t err = nvs_get_string(stringlify(TEST), str_key, buff, &buff_size);

    if (err != ESP_OK) {
        TEST_FAIL();
    }
}
