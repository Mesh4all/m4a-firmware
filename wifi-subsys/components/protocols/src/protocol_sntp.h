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
 *
 * @ingroup    wifi-subusys-protocols
 * @{
 * @file
 * @brief       sntp (Simple Network Time Protocol)
 * @author      eduazocar <eduardo@turpialdev.com>
 *
 */

#ifndef PROTOCOL_SNTP_H
#define PROTOCOL_SNTP_H

#include <time.h>

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Define a callback to every time function
 */
typedef void (*callback_time_t)(void);

/**
 * @brief This is a function to initialize the sntp server (This will not be taken, only have
 * to be applied inside of the function get_time_sntp())
 */
esp_err_t init_sntp(void);

/**
 * @brief main function that is working as task, this provides a sync with the sntp server
 * Receives a callback as param:
 * @param[in] params is a callback to provide a save_time or print current time
 *
 */

void get_time_sntp(void *params);

/**
 * @brief main function that is working as task, this provides a sync with the sntp server
 * Receives a callback as param:
 * @param[in] callback_sntp function that will processed in the sntp task
 *
 */

esp_err_t sntp_start(callback_time_t callback_sntp);

#ifdef __cplusplus
}
#endif
#endif /* PROTOCOL_SNTP_H */
/** @} */
