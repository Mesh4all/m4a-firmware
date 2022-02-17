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
static const char *TAG = "m4a-wifi-subsys";

void app_main(void) {

  wifi_init();

  printf("Init main m4a-subsys.\n");
}
