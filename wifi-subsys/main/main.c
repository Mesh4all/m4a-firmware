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
 * @brief       Main firmware file
 *
 * @copyright   Copyright (c) 2022 Mesh for all
 * @author      luisan00 <luisan00@hotmail.com>
 *
 */
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "httpsclient.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "wifi.h"
#include <esp_log.h>
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "storage.h"
#include "wifi.h"
#include "subsys_uart.h"
#include "icmp_ping.h"
#include "protocol_sntp.h"
#include "default_params.h"


esp_err_t set_default_values(void)
{
    esp_err_t err = nvs_set_string(stringlify(WAP), stringlify(WAP_SSID), WAP_SSID);
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to setting AP SSID to the nvs %s",
                esp_err_to_name(err));
    }

    err = nvs_set_string(stringlify(WAP), stringlify(WAP_PASS), WAP_PASS);
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to setting AP Password to the nvs %s",
                esp_err_to_name(err));
    }

    err = nvs_set_string(stringlify(WSTA), stringlify(WSTA_SSID), WSTA_SSID);
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to setting STA ssid to the nvs %s",
                esp_err_to_name(err));
    }

    err = nvs_set_string(stringlify(WSTA), stringlify(WSTA_PASS), WSTA_PASS);
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to setting STA password to the nvs %s",
                esp_err_to_name(err));
    }

    err = nvs_set_uint8(stringlify(WAP), stringlify(WAP_CHAN), WAP_CHAN);
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to setting WAP channel to the nvs %s",
                esp_err_to_name(err));
    }

    err = nvs_set_uint8(stringlify(WAP), stringlify(WAP_MAXCON), WAP_MAXCON);
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to setting AP max connections to the nvs %s",
                esp_err_to_name(err));
    }

    err = nvs_set_uint8(stringlify(WAP), stringlify(WAP_AUTH),WAP_AUTH);
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to setting AP auth to the nvs %s",
                esp_err_to_name(err));
    }
    err = nvs_set_uint8(stringlify(WIFI),
                        stringlify(WIFI_MODE),
                        identify_enabled_interface());
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to setting WIFI mode to the nvs %s",
                esp_err_to_name(err));
    }

    err = nvs_set_string(stringlify(I_CONECTION),
                        stringlify(SERVER_TO_PING),
                        SERVER_TO_PING);
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to server address to the nvs %s",
                esp_err_to_name(err));
    }


    err = nvs_set_uint8(stringlify(INTERNET_CONECTION),
                        stringlify(TIME_TO_PING),
                        TIME_TO_PING);
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to server address to the nvs %s",
                esp_err_to_name(err));
    }

    return err;
}

void icmp_callback(uint8_t internet_connected)
{
    if (internet_connected) {
        time_t now;
        struct tm now_info;
        char *format_time = "UTC";
        esp_err_t err;
        init_sntp();

        err = get_time_sntp(&now, &now_info, format_time);
        if (err != ESP_OK) {
            ESP_LOGE(__func__,
                    "Error to get time to sntp Reason: %s",
                    esp_err_to_name(err));
        }

        ESP_LOGI(__func__, "the time is %ld", (long)now);
    } else {
        ESP_LOGE(__func__,"Error: MCU is not connected to internet");
    }
}

void app_main(void)
{
    int8_t sta_connected = -1;
    ESP_LOGI(__func__, "starting wifi-subsys");
    esp_err_t err;

    err = nvs_init();
    if(err != ESP_OK) {
        ESP_LOGE(__func__,"error to init the nvs");
    }

    err = set_default_values();
    if(err != ESP_OK) {
        ESP_LOGE(__func__,"error to set default values to the nvs");
    }

    err = init_uart();
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Error: init uart failed %s", esp_err_to_name(err));
    }

#if WIFI_ENABLED
    err = wifi_init();
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Error: init uart failed %s", esp_err_to_name(err));
    }

    err = wifi_start(&sta_connected);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Error: init uart failed %s", esp_err_to_name(err));
    }

    if (sta_connected) {
       ESP_LOGI(__func__,"verifying internet connection");
       initialize_ping();
       manual_ping(&icmp_callback);
    }
#endif
}
