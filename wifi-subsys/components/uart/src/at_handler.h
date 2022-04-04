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
 * @ingroup    wifi_subsys_uart
 * @brief   AT handler
 * @{
 * @file
 * @author  xkevin190 <kevinvelasco193@gmail.com>
 */

#ifndef AT_HANDLER_H
#define AT_HANDLER_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @name All AT commands
 * @{
 */
enum at_keys_n {
    NVS_RST = 0,  /*!< Restore the subsystem to factory settings */
    WIFI_OFF,     /*!< Turn off the WiFi interface */
    WAP_SSID,     /*!< Set the AP SSID */
    WAP_PASS,     /*!< Set the AP password */
    WAP_CHAN,     /*!< Sets the AP channel */
    WAP_AUTH,     /*!< Sets the authentication type */
    WIFI_ON,      /*!< Turn on the WiFi interface */
    WIFI_MODE,    /*!< Sets the WiFi mode to None / AP / STA / AP+STA */
    WIFI_RST,     /*!< Restore the subsystem to factory settings */
    WSTA_SSID,    /*!< Set the STA SSID */
    WSTA_PASS,    /*!< Set the STA password */
    ATH,          /*!< this will enable at mode */
    ATHO,         /*!< this will disable at mode */
};
/** @} */

/**
 * @name this struct content the key and value of the AT request
 * @{
 */
typedef struct {
    char *value; /*!< AT value */
    char *key;   /*!< AT key */
    char action[2];/*!< AT action */
    char read_or_write[2]; /*!< AT type */
} at_request_t;
/** @} */

/**
 * @brief this function is a handler where will be identified AT commands
 * and it will executed  the functions for controlled  the wifi.
 *
 * @param at_commant [in] AT comands
 */
void at_handler(uint8_t* at_commant);

/**
 * @brief  this function parse the AT message is used for get the key and value of at request
 *
 * @param at_commant this is the message to parser
 * @param output    this variable content the key and value
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t parse_at_message(uint8_t* at_commant, at_request_t* output);

#ifdef __cplusplus
}
#endif
#endif /* AT_HANDLER_H */
/** @} */
