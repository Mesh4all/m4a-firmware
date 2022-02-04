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
 * @brief       uart module
 *
 * @copyright   Copyright (c) 2021 Mesh for all
 * @author      xkevin190 <kevinvelasco193@gmail.com>
 *
 */
#ifndef SUBSYS_UART_H
#define SUBSYS_UART_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif
/**
 * @brief this struct content the key and value of the AT request
 *
 */
typedef struct {
    char *value; /*!< AT value */
    char *key;   /*!< AT key */
} at_request_t;

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
};

/**
 * @brief this function initialize the uart
 * and init the event loop where will be received messages.
 *
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t init_uart();

/**
 * @brief this function is executed when arrived a new mensajes
 * a through uart rx, here identify if is an AT command or sensors data.
 *
 */
void rx_receive();

/**
 * @brief is one loop for send messages every  2 seconds
 * this function was created for send AT commands for testing
 *
 */
void tx_send_loop();
/**
 * @brief this function is executed when rx_receive function identify
 *  that the datos arrived were sensors values.
 *
 *  @param values [in] sensor values
 */
void received_sensor_data(uint8_t* values);

/**
 * @brief this function is a handler where will be identified AT commands
 * and it will executed  the functions for controlled  the wifi.
 *
 * @param at_comant [in] AT comands
 */
void at_handler(uint8_t* at_comant);
/**
 * @brief this function send the bytes through uart tx
 *
 * @return int  will be returned  the number of bytes sent
 */
int sendData();

/**
 * @brief in this function is added  the gpios
 * the baudrate  and set all the uart settings
 *
 */
esp_err_t setting_uart();

/**
 * @brief  this function parse the AT message is used for get the key and value of at request
 *
 * @param at_comant this is the message to parser
 * @param output    this variable content the key and value
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t parse_at_message(uint8_t* at_comant, at_request_t* output);

#ifdef __cplusplus
}
#endif
#endif /* SUBSYS_UART_H */
