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
 * @brief       AT COMMAND HANDLER
 *
 * @author      RocioRojas <rociorojas391@gmail.com>
 *
 */
#ifndef AT_UART_H
#define AT_UART_H

#define SHELL_BUFSIZE (128U)
#define UART_BUFSIZE (256U)

#include "ringbuffer.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief this is an enum used to define at commands.
 *
 */
 enum AT_COUNT {
    WAP_SSID = 0,  /*!< AP SSID*/
    GET_WAP_SSID,  /*!< GET AP SSID*/
    WAP_PASS,      /*!< AP PASSWORD*/
    GET_WAP_PASS,  /*!< GET AP PASSWORD*/
    WSTA_SSID,     /*!< STA SSID*/
    GET_WSTA_SSID, /*!< GET STA SSID*/
    WSTA_PASS,     /*!< STA PASSWORD*/
    GET_WSTA_PASS, /*!< GET STA PASSWORD*/
    WIFI_OFF,      /*!< WIFI OFF*/
    WIFI_ON,       /*!< WIFI ON */
    NVS_RST,       /*!< RESET NVS*/
    WIFI_MODE,     /*!< CHOOSE WIFI MODE*/
    GET_WIFI_MODE, /*!< GET WIFI MODE */
    WAP_CHAN,      /*!< CHOOSE AP CHANNEL */
    GET_WAP_CHAN,  /*!< GET AP CHANNEL */
    WAP_AUTH,      /*!< AP AUTH TYPE */
    GET_WAP_AUTH,  /*!< GET AP AUTH TYPE*/
    WIFI_RST,      /*!< RESET WIFI */
    ATH,           /*!< START AT */
    ATH0,          /*!< FINISH AT */
    MAX_AT,        /*!< MAX AT COMMANDS */
};
/**
 * @brief this is an struct used to define at commands, values, arguments and shell commands.
 *
 */
typedef struct {
    char *shell_cmd;  /*!< shell command */
    char *value_desc; /*!< argument description */
    char *at_cmd;     /*!< at command */
    uint8_t arg;      /*!< use 1 if command needs argument, 0 if don't.*/
    uint8_t response; /*!< use 1 if command waits for response, 0 if don't.*/
} at_list_t;
/**
 * @brief this is an struct used to define uart buffer.
 *
 */
typedef struct {
    char rx_mem[UART_BUFSIZE]; /*!< size of buffer */
    ringbuffer_t rx_buf;       /*!< define ring buffer */
    size_t count;              /*!< numbers of bytes received.*/
} uart_buffer_t;

/**
 * @brief  This function is a RX callback
 * @param [in] arg context to the callback (optional)
 * @param [in] data the byte that was received
 */
void rx_cb(void *arg, uint8_t data);

/**
 * @brief  This function is a handler of buffer
 * @param [in] arg context to the callback (optional)
 */
void *buff_handler(void *arg);

/**
 * @brief Search command listed in a at_list_t structure
 * @param [in] cmd  shell command
 * @return i command position
 *        -1 wrong command
 */
int at_search(char *cmd);

/**
 * @brief  This function handles at command
 * @return  0 Success
 *         -1 Failed
 */
int cmd_at(int argc, char **argv);

/**
 * @brief  Init uart, ring buffer, shell commands
 * @return 0 Success
 */
int config_at(void);

#ifdef __cplusplus
}
#endif
#endif /* AT_UART_H */
/** @} */
