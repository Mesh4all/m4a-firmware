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

esp_err_t init_sntp() {
    ESP_LOGI(__func__, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
    return ESP_OK;
}

esp_err_t get_time_sntp(time_t *curr_time, struct tm *curr_timeinfo, char *time_format) {
    char strftime_buf[64];
    esp_err_t err;
    if (curr_timeinfo->tm_year < (2016 - 1900)) {
        err = init_sntp();
        if (err != ESP_OK) {
            ESP_LOGE(__func__, "Failed to init the sntp protocol");
            return err;
        }
        time(curr_time);
    }

    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(__func__, "getting current time: (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(curr_time);
        localtime_r(curr_time, curr_timeinfo);
        if (retry == retry_count - 1) {
            ESP_LOGE(__func__, "Failed to sync with ntp server");
            sntp_stop();
            return ESP_FAIL;
        }
    }

    setenv("TZ", time_format, 1);
    tzset();
    localtime_r(curr_time, curr_timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", curr_timeinfo);
    ESP_LOGI(__func__, "The current date/time of %s is: %s\nTime posix: %lld ms", time_format,
             strftime_buf, (long long)*curr_time);

    // SNTP has to be off to avoid an system error when is called again //
    sntp_stop();
    return ESP_OK;
}
