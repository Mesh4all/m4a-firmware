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
 * @brief   Sntp protocol tests
 *
 * @author  eduazocar <eduardo@turpialdev.com>
 */

#include "esp_log.h"
#include "protocol_sntp.h"
#include "storage.h"
#include "unity.h"
#include "wifi.h"

bool wifi_initialized;

void save_time(void) {
    time_t saved_time;
    time(&saved_time);
    ESP_LOGI(__func__, "Time saved in NVS\nPosix:%ld", saved_time);
}

TEST_CASE("Get and set time by sntp", "[protocols]") {
    esp_err_t err;
    err = nvs_init();
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Reason: %s, No: %d, Line:%d, in file: %s", esp_err_to_name(err), err,
                 __LINE__, __FILE__);
    }
    if (!wifi_initialized) {
        err = wifi_init();
        if (err != ESP_OK) {
            ESP_LOGE(__func__, "Reason: %s, No: %d, Line:%d, in file: %s", esp_err_to_name(err),
                     err, __LINE__, __FILE__);
        }
        wifi_initialized = true;
    }
    err = sntp_start(&save_time);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Reason: %s, No %d, Line: %d, File: %s", esp_err_to_name(err), err,
                 __LINE__, __FILE__);
        TEST_FAIL();
    }
}
