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
#include "icmp_ping.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TEST_CASE("Wi-Fi initialize", "[network]")
{
    esp_err_t err;
    nvs_init();
    set_default_credentials();
    err = wifi_init();

    if (err != ESP_OK) TEST_FAIL();
}

TEST_CASE("setting ping session", "[network]")
{
   vTaskDelay(10000 / portTICK_PERIOD_MS);

   esp_err_t err = initialize_ping();
   if (err != ESP_OK) {
       TEST_FAIL();
   }
}

void icmp_callback(uint8_t is_connected)
{
    printf("nuevos parametros %d \n", is_connected);
}

TEST_CASE("send manual ping", "[network]")
{
   manual_ping(icmp_callback);
//    if (is_connected == 0) {
//        TEST_FAIL();
//    }
}

TEST_CASE("get current status", "[network]")
{
   int is_connected = get_current_status();
   if (is_connected == 0) {
       TEST_FAIL();
   }
}
