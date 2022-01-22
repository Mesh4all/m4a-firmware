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
 * @brief       Wifi module
 * @ingroup     wifisubsys
 * @{
 * @copyright   Copyright (c) 2021 Mesh for all
 * @author      xkevin190 <kevinvelasco193@gmail.com>
 *
 */
#ifndef WIFI_H
#define WIFI_H

#include "esp_wifi.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief this is main function of the Wifi module,
 * this starting the esp_netif the handler for the wifi events
 * and verify if will be start the ap or sta.
 *
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t wifi_init();

/**
 * @brief this function verifies if the wifi is in sta mode
 *
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
int is_sta();

/**
 * @brief this function verifies if the wifi is in ap mode
 *
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
int is_ap();

/**
 * @brief this function initializes the AP. in this
 * function is added wifi ssid and password and start
 * the ap mode.
 *
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t init_ap();

/**
 * @brief this function initializes the STA and it
 * is added ssid and password and start
 * the sta mode.
 *
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t init_sta();

/**
 * @brief Set the default credentials ssid and password of AP and STA
 *
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t set_default_credentials();

/**
 * @brief verify if the wifi ssid and password is configured in the storage
 *
 * @return 1 when the wifi module is configured
 *         0 when the wifi is not configured
 */
uint8_t wifi_credentials_are_configured();

#ifdef __cplusplus
}
#endif
#endif /* WIFI_H */
/** @} */
