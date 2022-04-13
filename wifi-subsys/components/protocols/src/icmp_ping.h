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
 * @ingroup wifi-subusys-protocols
 * @{
 * @file
 * @brief       icmp module
 * @author      xkevin190 <kevinvelasco193@gmail.com>
 *
 */
#ifndef ICMP_PING_H
#define ICMP_PING_H

#include "esp_wifi.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief this is a callback will return the current status
 *
 */
typedef void (*callback_t)(uint8_t);

/**
 * @brief init wifi and start a new task
 *  where send a ping every 60 seconds
 *
 * @return esp_err_t
 */
esp_err_t enable_ping(void);

/**
 * @brief  this function manually sends the ping
 *
 * @return 1 when is connected
 *         0 when is not connected
 */
void manual_ping(callback_t callback);

/**
 * @brief  get connection status
 *
 * @return 1 when is connected
 *         0 when is not connected
 */
uint8_t get_current_status(void);

/**
 * @brief  this function initializes the ping session
 *
 * @return esp_err_t
 */
esp_err_t initialize_ping(void);

#ifdef __cplusplus
}
#endif
#endif /* ICMP_PING_H */
/** @} */
