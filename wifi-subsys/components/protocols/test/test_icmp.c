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
#include "default_params.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "icmp_ping.h"
#include "storage.h"
#include "unity.h"
#include "wifi.h"

esp_err_t set_default_ping_settings(void) {
    ESP_LOGI(__func__, "Default ping settings");
    esp_err_t err = nvs_set_string(stringlify(PING), stringlify(PING_TO), PING_TO);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Error: setting ping address to the nvs %s", esp_err_to_name(err));
    }
    err = nvs_set_uint32(stringlify(PING), stringlify(PING_INT), PING_INT);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Error: setting ping interval to the nvs %s", esp_err_to_name(err));
    }
    err = nvs_set_uint8(stringlify(PING), stringlify(PING_RET), PING_RET);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Error: setting ping retries to the nvs %s", esp_err_to_name(err));
    }
    err = nvs_set_uint32(stringlify(PING), stringlify(PING_TIMEOUT), PING_TIMEOUT);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Error: setting ping timeout to the nvs %s", esp_err_to_name(err));
    }
    if (err != ESP_OK) {
        nvs_set_uint8(stringlify(NVS), stringlify(NVS_IS_INIT), 0);
        return err;
    } else {
        nvs_set_uint8(stringlify(NVS), stringlify(NVS_IS_INIT), 1);
        return ESP_OK;
    }
}

TEST_CASE("Wi-Fi initialize", "[network]") {
    esp_err_t err;
    nvs_init();
    err = set_default_credentials();
    if (err != ESP_OK) {
        TEST_FAIL();
    };
    err = wifi_init();
    if (err != ESP_OK) {
        TEST_FAIL();
    };

    wifi_start(NULL);
}

TEST_CASE("setting ping session", "[network]") {
    set_default_ping_settings();
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    initialize_ping();
}

void icmp_callback(uint8_t is_connected) { printf("nuevos parametros %d \n", is_connected); }
TEST_CASE("send manual ping", "[network]") {
    nvs_init();
    esp_err_t err = set_default_credentials();
    if (err != ESP_OK) {
        TEST_FAIL();
    };
    manual_ping(icmp_callback);
}
TEST_CASE("start ping task", "[network]") {
    nvs_init();
    esp_err_t err = set_default_ping_settings();
    if (err != ESP_OK) {
        TEST_FAIL();
    };
    enable_ping();
}

TEST_CASE("get current status", "[network]") {
    int is_connected = get_current_status();
    if (is_connected == 0) {
        TEST_FAIL();
    }
}
