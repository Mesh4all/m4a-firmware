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

#include "unity.h"
#include "wifi.h"
#include "storage.h"
#include "esp_log.h"
#include "httpsclient.h"

uint8_t wifi_initialized = 0;

void callback(http_response_t *response) {
    ESP_LOGI(__func__, "executed callback the response: %d !!! %s", response->status,
             response->output);
}

TEST_CASE("https get request", "[protocols]") {
    esp_err_t err = nvs_init();
    if (err == ESP_OK) {
        ESP_LOGI(__func__, "Setting NVS");
    }
    err = set_default_credentials();
    if (err == ESP_OK) {
        ESP_LOGI(__func__, "Setting default credentials");
    }
    if (!wifi_initialized) {
        err = wifi_init();
        wifi_initialized = 1;
        if (err == ESP_OK) {
            ESP_LOGI(__func__, "Setting wifi");
        }
    }
    wifi_start(NULL);
    http_request_t request;
    request.method = HTTP_METHOD_GET;
    request.content_type = HTTPS_CONTENT_JSON;
    memcpy(request.url, "https://cloud.mesh4all.org/?a=1&b=2&c=3",
           sizeof("https://cloud.mesh4all.org/?a=1&b=2&c=3"));
    request.callback = &callback;
    http_client(&request);
}

TEST_CASE("https post request", "[protocols]") {
    esp_err_t err = nvs_init();
    if (err == ESP_OK) {
        ESP_LOGI(__func__, "Setting NVS");
    }
    err = set_default_credentials();
    if (err == ESP_OK) {
        ESP_LOGI(__func__, "Setting default credentials");
    }
    if (!wifi_initialized) {
        err = wifi_init();
        wifi_initialized = 1;
        if (err == ESP_OK) {
            ESP_LOGI(__func__, "Setting wifi");
        }
    }
    wifi_start(NULL);
    http_request_t request;
    request.method = HTTP_METHOD_POST;
    memcpy(request.url, "https://cloud.mesh4all.org", sizeof("https://cloud.mesh4all.org"));
    memcpy(request.body, "{\"name\":\"value\"}", sizeof("{\"name\":\"value\"}"));
    request.content_type = HTTPS_CONTENT_JSON;
    request.callback = &callback;
    http_client(&request);
    vTaskDelay(50 / portTICK_PERIOD_MS);
}
