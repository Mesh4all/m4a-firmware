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
 * @brief   UART_AT
 *
 * @author  eduazocar <eduardo@turpialdev.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"

#include "msg.h"
#include "shell.h"
#include "thread.h"

#include "periph/uart.h"
#include "xtimer.h"

#define AT_UART UART_DEV(1)

#define AT_UART_BAUDRATE 115200
#define MAX_SENTENCE_SIZE 128

#define MAIN_QUEUE_SIZE (8)
msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

size_t count = 0;
char uart_buff[MAX_SENTENCE_SIZE] = {0};
char *commands_at[] = {
    "AT+WAP_SSID=", "AT+WAP_PASS=",  "AT+WSTA_SSID=", "AT+WSTA_PASS=", "AT+WIFI_OFF", "AT+WIFI_ON",
    "AT+NVS_RST",   "AT+WIFI_MODE=", "AT+WAP_CHAN=",  "AT_WAP_AUTH=",  "AT+WIFI_RST"};

static void at_uart_rx_cb(void *arg, uint8_t c) {
    if (c != 10) {
        uart_buff[count] = (char)c;
        count += 1;
    } else {
        printf("%s\n", (char *)uart_buff);
        memset(uart_buff, 0, sizeof(uart_buff));
        count = 0;
    }
}

void send_at(char *at_command, char *param_at) {
    char strbuf[64];
    strcpy(strbuf, at_command);
    if (strcmp(param_at, "_EMPTY_") == 0) {
        uart_write(AT_UART, (uint8_t *)strbuf, strlen(strbuf) + 1);
    } else {
        strcat(strbuf, param_at);
        uart_write(AT_UART, (uint8_t *)strbuf, strlen(strbuf) + 1);
    }
    uart_poweroff(AT_UART);
}

int cmd_uart_at(int argc, char **argv) {
    uart_init(AT_UART, AT_UART_BAUDRATE, at_uart_rx_cb, NULL);
    if (argc < 2) {
        printf("Usage: \n--help\n");
        return -1;
    }

    else if (strcmp("on", argv[1]) == 0) {
        send_at(commands_at[5], "_EMPTY_");

    } else if (strcmp("off", argv[1]) == 0) {
        send_at(commands_at[4], "_EMPTY_");
        uart_poweroff(AT_UART);
        return 0;
    }

    /* Settings of mode AP */
    if (strcmp("ap", argv[1]) == 0) {
        if (argc < 3) {
            printf("\nWifi ap settings:\n\tssid\n\tpswd\n\tauth\n\tchannel\n");
            return 0;

        } else if (strcmp(argv[2], "ssid") == 0) {
            if (argc < 4) {
                printf("\nPlease enter your SSID name to Wifi mode ap settings\n*Remember use "
                       "least of 32 characters*\n");
                return 0;
            } else if (strcmp(argv[3], "_EMPTY_") != 0) {
                send_at(commands_at[2], argv[3]);
            }
        } else if (strcmp(argv[2], "pswd") == 0) {
            if (argc < 4) {
                printf("\nPlease enter your password to Wifi mode ap settings\n");
                return 0;
            } else if (strcmp(argv[3], "") != 0) {
            }

        } else if (strcmp(argv[2], "auth") == 0) {
            if (argc != 4) {
                printf("\nDefine the authentication mode to Wifi mode ap settings\n");
                return 0;
            } else if (strcmp(argv[3], "_EMPTY_") != 0) {
            }

        } else if (strcmp(argv[2], "channel") == 0) {
            if (argc != 4) {
                printf("\nChoose the channel <1 ... 7> to Wifi mode ap settings\n");
                return 0;

            } else if (strcmp(argv[3], "_EMPTY_") != 0) {
            }
        }
        return 0;
    }

    /* Settings of STA mode */
    else if (strcmp("sta", argv[1]) == 0) {
        if (argc < 3) {
            printf("\nWifi ap settings:\n\tssid\n\tpswd\n\tauth\n\tchannel\n");
            return 0;
        } else if (strcmp(argv[2], "ssid") == 0) {
            if (argc < 4) {
                printf("\nPlease enter your SSID name to Wifi mode ap settings\n*Remember use "
                       "least of 32 characters*\n");
                return 0;
            } else if (strcmp(argv[3], "_EMPTY_") != 0) {
            }
        } else if (strcmp(argv[2], "pswd") == 0) {
            if (argc < 4) {
                printf("\nPlease enter your password to Wifi mode ap settings\n");
                return 0;
            } else if (strcmp(argv[3], "_EMPTY_") != 0) {

                return 0;
            }
        }
    } else if (strcmp("mode", argv[1]) == 0) {
        if (argc != 3) {
        }
    }
    return -1;
}

static const shell_command_t shell_commands[] = {{"wifi", "Wifi settings", cmd_uart_at},
                                                 {NULL, NULL, NULL}};

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("Welcome to RIOT!");

    /* run the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
