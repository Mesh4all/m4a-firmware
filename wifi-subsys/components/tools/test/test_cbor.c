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
 * @brief   Non Volatile Storage
 *
 * @author  eduazocar <eduardo@turpialdev.com>
 */
#include <stdio.h>
#include <stdint.h>
#include "esp_log.h"
#include "unity.h"
#include "serialization.h"

static char* key_element[] = {"Key test", "Key_1", "Key_2"};
size_t len;
uint8_t cbor_code[20];
int64_t val_origin;
int64_t val_received;

TEST_CASE("Encode a int value = 34 of an key_element[0]", "cbor")
{
    val_origin = 34;
    esp_err_t err = encode(key_element[0], val_origin, cbor_code, &len);

    if (err != ESP_OK)
    {
        TEST_FAIL();
    }
}

TEST_CASE("Encode a int value = -356 of an key_element[1]", "cbor")
{
    val_origin= -356;
    esp_err_t err = encode(key_element[1], val_origin, cbor_code, &len);

    if (err != ESP_OK)
    {
        TEST_FAIL();
    }
}

TEST_CASE("Encode a int value = 16 of an key_element[2]", "cbor")
{
    val_origin = 16;
    esp_err_t err = encode(key_element[2], val_origin, cbor_code, &len);

    if (err != ESP_OK)
    {
        TEST_FAIL();
    }
}

TEST_CASE("Decode the int value that belongs to key_element[0] ", "cbor")
{
    esp_err_t err = decode((uint8_t*)cbor_code, key_element[0], &val_received, len);

    if (err != ESP_OK) {
        ESP_LOGE("ERROR ", "NO READ");
        TEST_FAIL();
    }

    if (val_received != val_origin) {
        ESP_LOGE("ERROR ", "Wrong Value of the origin");
        TEST_FAIL();
    }

    val_received = 0;

}

TEST_CASE("Decode the int value that belongs to key_element[1]", "cbor")
{
    ESP_LOGI("R-", "%s", (char *)cbor_code);
    esp_err_t err = decode((uint8_t*)cbor_code, key_element[1], &val_received, len);

    if (err != ESP_OK) {
        ESP_LOGE("ERROR ", "NO READ");
        TEST_FAIL();
    }

    if (val_received != val_origin) {
        ESP_LOGE("ERROR ", "Wrong Value of the origin");
        TEST_FAIL();
    }

    val_received = 0;

}

TEST_CASE("Decode the int value that belongs to key_element[2]", "cbor")
{
    ESP_LOGI("R-", "%s", (char *)cbor_code);
    esp_err_t err = decode((uint8_t*)cbor_code, key_element[2], &val_received, len);

    if (err != ESP_OK ) {
        ESP_LOGE("ERROR ", "NO READ");
        TEST_FAIL();
    }

    if (val_received != val_origin) {
        ESP_LOGE("ERROR ", "Wrong Value of the origin");
        TEST_FAIL();
    }

    val_received = 0;

}
