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
 * @brief   Wifi module
 *
 * @author  xkevin190 <kevinvelasco193@gmail.com>
 */

#include "unity.h"
#include "wifi.h"
#include "storage.h"

TEST_CASE("set default credentials", "[network]") {
    nvs_init();
    esp_err_t err = set_default_credentials();
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("Wi-Fi initialize", "[network]") {
    int8_t is_connected = -1;
    esp_err_t err;
    err = wifi_init();

    if (err != ESP_OK) {
        TEST_FAIL();
    }
    err = wifi_start(&is_connected);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("change ssid ap", "[network]") {
    const char *test_ap = "test-ssid";
    esp_err_t err = change_wifi_ap_ssid(test_ap);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("change password ap", "[network]") {
    const char *test_ap = "default_password";
    esp_err_t err = change_wifi_ap_pass(test_ap);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("change ssid sta", "[network]") {
    const char *test_sta = "Chicho";
    esp_err_t err = change_wifi_sta_ssid(test_sta);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("change password ap", "[network]") {
    const char *test_sta = "Pirulin0312";
    esp_err_t err = change_wifi_sta_pass(test_sta);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("change channel", "[network]") {
    esp_err_t err = select_wifi_channel(3);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("change max_connections", "[network]") {
    esp_err_t err = set_ap_max_connection(2);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("change auth ap", "[network]") {
    esp_err_t err = change_ap_auth(WIFI_AUTH_WPA2_PSK);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("change STA mode", "[network]") {
    esp_err_t err = change_wifi_mode(1);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("change AP mode", "[network]") {
    esp_err_t err = change_wifi_mode(2);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("change APSTA mode", "[network]") {
    esp_err_t err = change_wifi_mode(3);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("WIFI OFF", "[network]") {
    esp_err_t err = wifi_off();
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}

TEST_CASE("WIFI ON", "[network]") {
    esp_err_t err = wifi_init();
    wifi_start(NULL);
    if (err != ESP_OK) {
        TEST_FAIL();
    }
}
