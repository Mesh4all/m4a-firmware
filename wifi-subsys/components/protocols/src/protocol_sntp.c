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
 * @brief   SNTP (Simple Network Time Protocol)
 *
 * @author  eduazocar <eduardo@turpialdev.com>
 */
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "default_params.h"
#include "esp_attr.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_sntp.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "protocol_sntp.h"
#include "storage.h"

esp_err_t init_sntp() {
    ESP_LOGI(__func__, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
    return ESP_OK;
}

void get_time_sntp(void *params) {
    callback_time_t callback = (callback_time_t)params;
    time_t now;
    const int retry_count = SNTP_MAX_COUNT;
    int retry = 0;
    while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED && ++retry <= retry_count) {
        ESP_LOGI(__func__, "getting current time: (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        if (retry == retry_count) {
            ESP_LOGE(__func__, "Failed to sync with ntp server");
        }
    }
    callback();
    // SNTP has to be off to avoid an system error when is called again //
    sntp_stop();
    vTaskDelete(NULL);
}

esp_err_t sntp_start(callback_time_t callback_sntp) {
    esp_err_t err = init_sntp();
    xTaskCreate(&get_time_sntp, "Sntp_task", 1024 * 2, callback_sntp, 1, NULL);
    return err;
}
