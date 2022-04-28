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
 * @brief      Uart module
 * @{
 * @file
 * @author      xkevin190 <kevinvelasco193@gmail.com>
 *
 *
 */
#ifndef SUBSYS_UART_H
#define SUBSYS_UART_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

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
void received_sensor_data(uint8_t *values, size_t len);

/**
 * @brief this function is used to send data through uart
 *
 * @param logName [in] logName
 * @param data [in] data
 */
int sendData(const char *logName, char *data);

/**
 * @brief in this function is added  the gpios
 * the baudrate  and set all the uart settings
 *
 */
esp_err_t setting_uart();

#ifdef __cplusplus
}
#endif
#endif /* SUBSYS_UART_H */
/* @} */
