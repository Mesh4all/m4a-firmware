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
#include <string.h>
#include "unity.h"
#include "subsys_uart.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "uart_test";

TEST_CASE("uart initialize", "[uart]") {
    esp_err_t err;

    err = init_uart();

    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("send uart bytes", "[uart]") {
    const char *test = "test_data";
    int byteslen = sendData("uart_test", test);

    if (byteslen == 0) {
        TEST_FAIL();
    }
}

TEST_CASE("parse AT message", "[uart]") {
    at_request_t payload;
    payload.key = malloc(32);
    payload.value = malloc(32);

    uint8_t test_message[50] = "AT+WSTA_SSID=new_ssid";
    parse_at_message(test_message, &payload);

    ESP_LOGI(TAG, " the key is %s", payload.key);
    if (strcmp(payload.key, "WSTA_SSID") != 0) {
        TEST_FAIL();
    }
    ESP_LOGI(TAG, " the value is is %s", payload.key);
    if (strcmp(payload.value, "new_ssid") != 0) {
        TEST_FAIL();
    }
}

TEST_CASE("turn on wifi since at handler", "[uart]") {
    char *test_message = "AT+WIFI_ON";

    at_handler((uint8_t *)test_message);
}

TEST_CASE("restore default params to wifi", "[uart]") {
    char *test_message = "AT+NVS_RST";

    at_handler((uint8_t *)test_message);
}

TEST_CASE("change wifi ap ssid", "[uart]") {
    char *test_message = "AT+WAP_SSID=test_ssid12";

    at_handler((uint8_t *)test_message);
}

TEST_CASE("change wifi ap password", "[uart]") {
    char *test_message = "AT+WAP_PASS=test_password";

    at_handler((uint8_t *)test_message);
}

TEST_CASE("change wifi ap ssid", "[uart]") {
    char *test_message = "AT+WSTA_SSID=Chicho";

    at_handler((uint8_t *)test_message);
}

TEST_CASE("change wifi sta password", "[uart]") {
    char *test_message = "AT+WSTA_PASS=Pirulin0312";

    at_handler((uint8_t *)test_message);
}

TEST_CASE("restore default params to wifi", "[uart]") {
    char *test_message = "AT+NVS_RST";

    at_handler((uint8_t *)test_message);
}

TEST_CASE("set ap mode", "[uart]") {
    char *test_message = "AT+WIFI_MODE=2";

    at_handler((uint8_t *)test_message);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

TEST_CASE("set sta mode", "[uart]") {
    char *test_message = "AT+WIFI_MODE=1";

    at_handler((uint8_t *)test_message);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

TEST_CASE("set sta mode", "[uart]") {
    char *test_message = "AT+WIFI_MODE=3";

    at_handler((uint8_t *)test_message);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

TEST_CASE("set sta mode", "[uart]") {
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    char *test_message = "AT+WIFI_MODE";

    at_handler((uint8_t *)test_message);
}
