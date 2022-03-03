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
 * @ingroup     wifi-subsys_components
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

/**
 * @brief change the ssid of the sta and restart the wifi module
 *
 * @param sta_ssid [in] new ssid
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t change_wifi_sta_ssid(char* sta_ssid);

/**
 * @brief change the ssid of the AP and restart the wifi module
 *         also save new ssid in the nvs
 *
 * @param sta_ssid [in] new AP ssid
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t change_wifi_ap_ssid(char* ap_ssid);

/**
 * @brief change the password of the AP and restart the wifi module
 *        also save new password in the nvs
 *
 * @param sta_ssid [in] new AP password
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t change_wifi_ap_pass(char* pass_ap);

/**
 * @brief change the password of the STA and restart the wifi module
 *        also save new password in the nvs
 *
 * @param pass_sta [in] new sta password
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t change_wifi_sta_pass(char* pass_sta);

/**
 * @brief restart the wifi module
 *
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t wifi_restart();

/**
 * @brief turn off the WIFI module
 *
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t wifi_off();

/**
 * @brief turn on the WIFI module
 *
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t wifi_on();

/**
 * @brief is used when the default parameters still not save in storage,
 *        this verify that wifi mode is enabled in the default params and
 *         will return the mode.
 *
 * @return int
 */
int identify_enabled_interface();

/**
 * @brief this function set the wifi mode and execute the connection,
 *         it find the wifi mode in the nvs and it verify if already saved
 *         the mode in the nvs. If the nvs return error or the wifi_mode is
 *         equal to WIFI_MODE_NULL will take the defaults params for give
 *         warranty to the user that wifi init correctly this is very important
 *         because if something failed  the wifi  will  stop.
 *
 */
esp_err_t set_wifi_mode();

/**
 * @brief change type of auth in ap wifi
 *
 * @param auth [in]
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t change_ap_auth (uint8_t auth);

/**
 * @brief set new mode in the nvs and start the wifi again.
 *
 * @param mode [in]
 * @return esp_err_t
 */
esp_err_t change_wifi_mode(int mode);

/**
 * @brief restore WiFi parameters to their original state
 *
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t wifi_restore_default();

/**
 * @brief select the channel of wifi ap is recommended see the doc
 * of idf for the channels every channel have a different frequency
 *
 * @param channel [in]
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t select_wifi_channel(uint8_t channel);

/**
 * @brief Set the ap max number of connections
 *
 * @param max_connection [in]  max connectios
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t set_ap_max_connection (uint8_t max_connection);

esp_err_t wifi_start(int8_t *is_connected);
int8_t wifi_bit_event (void);

#ifdef __cplusplus
}
#endif
#endif /* WIFI_H */
/** @} */
