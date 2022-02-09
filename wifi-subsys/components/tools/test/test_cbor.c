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

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "serialization.h"
#include "unity.h"

static char *key_element[] = {"Key test", "Key_1", "Key_2", "Array_control"};
size_t len;
uint8_t cbor_code[50];
int64_t val_origin;
int64_t val_received;
uint8_t array_inp[] = {25, 15, 1};
uint8_t *array_out;

// *********************** ENCODE SECTION TESTS ++++++++++++++++++++++++++++//

TEST_CASE("Encode a int value = 34 of an key_element[0]", "cbor") {

    esp_err_t err = encode(key_element[0], val_origin, cbor_code, &len);

    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Reason %s, No(%d) in the line:%d by the file %s", esp_err_to_name(err),
                 err, __LINE__, __FILE__);
        TEST_FAIL();
    }
}

TEST_CASE("Encode a int value = -356 of an key_element[1]", "cbor") {

    val_origin = -356;
    esp_err_t err = encode(key_element[1], val_origin, cbor_code, &len);

    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Reason %s, No(%d) in the line:%d by the file %s", esp_err_to_name(err),
                 err, __LINE__, __FILE__);
        TEST_FAIL();
    }
}

TEST_CASE("Encode a int value = 16 of an key_element[2]", "cbor") {

    val_origin = 16;
    esp_err_t err = encode(key_element[2], val_origin, cbor_code, &len);

    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Reason %s, No(%d) in the line:%d by the file %s", esp_err_to_name(err),
                 err, __LINE__, __FILE__);
        TEST_FAIL();
    }
}

TEST_CASE("Encode an array of some values uint8_t key_element[3]", "cbor") {

    esp_err_t err = encode_uint8(key_element[3], array_inp, cbor_code, &len);

    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Reason %s, No(%d) in the line:%d by the file %s", esp_err_to_name(err),
                 err, __LINE__, __FILE__);
        TEST_FAIL();
    }
}

//*********************** DECODE SECTION TESTS*******************************//

TEST_CASE("Decode the int value that belongs to key_element[0]", "cbor") {

    esp_err_t err = decode((uint8_t *)cbor_code, key_element[0], &val_received, len);
    ESP_LOGI("The value from origin is ", "%lld --> The value received is %lld",
             (long long)val_origin, (long long)val_received);

    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Reason %s, No(%d) in the line:%d by the file %s", esp_err_to_name(err),
                 err, __LINE__, __FILE__);
        TEST_FAIL();
    }

    if (val_received != val_origin) {
        ESP_LOGE(__func__, "Reason %s, No(%d) in the line:%d by the file %s", esp_err_to_name(err),
                 err, __LINE__, __FILE__);
        TEST_FAIL();
    }

    val_received = 0;
}

TEST_CASE("Decode the int value that belongs to key_element[1]", "cbor") {

    esp_err_t err = decode((uint8_t *)cbor_code, key_element[1], &val_received, len);
    ESP_LOGI("The value from origin is ", "%lld --> The value received is %lld",
             (long long)val_origin, (long long)val_received);

    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Reason %s, No(%d) in the line:%d by the file %s", esp_err_to_name(err),
                 err, __LINE__, __FILE__);
        TEST_FAIL();
    }

    if (val_received != val_origin) {
        ESP_LOGE(__func__, "Reason %s, No(%d) in the line:%d by the file %s", esp_err_to_name(err),
                 err, __LINE__, __FILE__);
        TEST_FAIL();
    }

    val_received = 0;
}

TEST_CASE("Decode the int value that belongs to key_element[2]", "cbor") {

    esp_err_t err = decode((uint8_t *)cbor_code, key_element[2], &val_received, len);
    ESP_LOGI("The value from origin is ", "%lld --> The value received is %lld",
             (long long)val_origin, (long long)val_received);

    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Reason %s, No(%d) in the line:%d by the file %s", esp_err_to_name(err),
                 err, __LINE__, __FILE__);
        TEST_FAIL();
    }

    if (val_received != val_origin) {
        ESP_LOGE(__func__, "Reason %s, No(%d) in the line:%d by the file %s", esp_err_to_name(err),
                 err, __LINE__, __FILE__);
        TEST_FAIL();
    }

    val_received = 0;
}

TEST_CASE("Decode the uint8_t values that belongs to key_element[3]", "cbor") {

    array_out = malloc(sizeof(len));
    esp_err_t err = decode((uint8_t *)cbor_code, key_element[3], array_out, len);

    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Reason %s, No(%d) in the line:%d by the file %s", esp_err_to_name(err),
                 err, __LINE__, __FILE__);
        TEST_FAIL();
    }

    if (memcmp(array_inp, array_out, sizeof(array_inp)) != 0) {
        ESP_LOGE(__func__, "Reason %s, No(%d) in the line:%d by the file %s", esp_err_to_name(err),
                 err, __LINE__, __FILE__);
        TEST_FAIL();
    }

    free(array_out);
    val_received = 0;
}
