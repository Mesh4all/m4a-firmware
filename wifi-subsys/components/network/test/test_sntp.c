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

TEST_CASE("Get and set time by sntp", "[network][protocols][sntp]") {
    esp_err_t err;
    if (!wifi_initialized) {
        wifi_init();
        wifi_initialized = true;
    }
    time_t now;
    struct tm now_info;
    char *format_time = "UTC+4";
    err = get_time_sntp(&now, &now_info, format_time);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Reason: %s, No %d, Line: %d, File: %s", esp_err_to_name(err), err,
                 __LINE__, __FILE__);
        TEST_FAIL();
    }
}
