/*
 * Copyright (c) 2021 Mesh4all <mesh4all.org>
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
 * @author      luisan00 <luisan00@hotmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"

#include "shell.h"
#include "thread.h"
#include "msg.h"

#include "periph/uart.h"
#include "xtimer.h"

#ifdef BOARD_STM32F469I_DISCO
#define GPS_UART UART_DEV(1)
#elif BOARD_M4A_MB
#define GPS_UART UART_DEV(2)
#endif

#define GPS_BAUDRATE 115200
#define MAX_SENTENCE_SIZE 128

#define MAIN_QUEUE_SIZE (8)
msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

size_t count = 0;
char uart_buff[MAX_SENTENCE_SIZE] = {0};

static void gps_rx_cb(void *arg, uint8_t c)
{
    if (c != 10)
    {
        uart_buff[count] = (char)c;
        count += 1;
    }
    else
    {
        printf("%s\n", (char *)uart_buff);
        memset(uart_buff, 0, sizeof(uart_buff));
        count = 0;
    }
}

int cmd_gps(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: \nuart start|stop\n");
        return -1;
    }

    if (strcmp("start", argv[1]) == 0)
    {
        return uart_init(GPS_UART, GPS_BAUDRATE, gps_rx_cb, NULL);
    }
    if (strcmp("stop", argv[1]) == 0)
    {
        uart_poweroff(GPS_UART);
        return 0;
    }
    printf("Usage: \nuart start|stop\n");
    return -1;
}

static const shell_command_t shell_commands[] = {
    {"gps", "start or stop reading the GPS uart port", cmd_gps},
    {NULL, NULL, NULL}};

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("Welcome to RIOT!");

    /* run the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
