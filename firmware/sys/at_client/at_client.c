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
 * @brief  AT COMMAND HANDLER
 *
 * @author  RocioRojas <rociorojas391@gmail.com>
 * @author  eduazocar <eduardo@turpialdev.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "at_client.h"
#include "board.h"
#include "msg.h"
#include "periph/uart.h"
#include "shell.h"
#include "thread.h"
#include "timex.h"
#include "xtimer.h"

#define AT_UART UART_DEV(1)
#define AT_BAUDRATE 115200

#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define QUEUE_SIZE (8)
#define WAIT_RESPONSE (10 * US_PER_SEC) // time in microseconds

kernel_pid_t buff_thread_pid;
char stack[THREAD_STACKSIZE_MAIN];
uart_buffer_t uart_buffer = {
    .rx_mem = {0},
    .count = 0,
};
uint8_t flag_response = 0;

at_list_t at_list[] = {
    // shell command, value description, at command, argument, response
    {"wap_ssid", "<AP SSID>", "AT+WAP_SSID=", 1, 1},
    {"get_wap_ssid", "", "AT+WAP_SSID?", 0, 1},
    {"wap_pass", "<AP Password>", "AT+WAP_PASS=", 1, 1},
    {"get_wap_pass", "", "AT+WAP_PASS?", 0, 1},
    {"wsta_ssid", "<STA SSID>", "AT+WSTA_SSID=", 1, 1},
    {"get_wsta_ssid", "", "AT+WSTA_SSID?", 0, 1},
    {"wsta_pass", "<STA Password>", "AT+WSTA_PASS=", 1, 1},
    {"get_wsta_pass", "", "AT+WSTA_PASS?", 0, 1},
    {"wifi_off", "", "AT&WIFI_OFF", 0, 1},
    {"wifi_on", "", "AT&WIFI_ON", 0, 1},
    {"nvs_rst", "", "AT&NVS_RST", 0, 1},
    {"wifi_mode", "<0 / 1 / 2 / 3>", "AT+WIFI_MODE=", 1, 1},
    {"get_wifi_mode", "", "AT+WIFI_MODE?", 0, 1},
    {"wap_chan", "<Number>", "AT+WAP_CHAN=", 1, 1},
    {"get_wap_chan", "", "AT+WAP_CHAN?", 0, 1},
    {"wap_auth", "<Number as AUTH_TYPE>", "AT+WAP_AUTH=", 1, 1},
    {"get_wap_auth", "", "AT+WAP_AUTH?", 0, 1},
    {"wifi_rst", "", "AT&WIFI_RST", 0, 1},
    {"ath", "", "ATH", 0, 0},
    {"ath0", "", "ATH0", 0, 0},
    {"at", "", "AT", 0, 1},
    {NULL, NULL, NULL, 0, 0},
};

void rx_cb(void *arg, uint8_t data) {
    (void)arg;
    if (flag_response) {
        ringbuffer_add_one(&uart_buffer.rx_buf, data);
        uart_buffer.count += 1;
    }
}

void *buff_handler(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t msg_queue[QUEUE_SIZE];
    char byte;
    msg_init_queue(msg_queue, QUEUE_SIZE);
    while (1) {
        msg_receive(&msg);
        printf("RX: ");
        if (ringbuffer_empty(&(uart_buffer.rx_buf)) == 0) {
            do {
                byte = (int)ringbuffer_get_one(&(uart_buffer.rx_buf));
                uart_buffer.count -= 1;
                if (byte >= ' ' && byte <= '~') {
                    printf("%c", byte);
                } else {
                    printf("x%02x", (unsigned char)byte);
                }
            } while (uart_buffer.count > 0 && byte != 0);
            memset(uart_buffer.rx_mem, 0, UART_BUFSIZE); // clean residual buffer
        }
    }
    return NULL; // this should never be reached
}

int at_search(char *cmd) {
    for (int i = 0; i < MAX_AT; i++) {
        if (strcmp(at_list[i].shell_cmd, cmd) == 0) {
            return i;
        }
    }
    return -1;
}

int cmd_at(int argc, char **argv) {

    msg_t msg_send, msg_receive;
    xtimer_t wait_timer;
    char buff_at[50] = {0};
    if (argc < 2) {
        printf("Use command \"at help\" for more info.\n");
        return -1;
    }
    if (strcmp("help", argv[1]) == 0) {
        printf("Valid commands are: \n\n");
        for (int i = 0; i < MAX_AT; i++) {
            printf("%s %s \n", at_list[i].shell_cmd, at_list[i].value_desc);
        }
        return 0;
    }
    int at_position = at_search(argv[1]);
    if (at_position < 0) {
        return -1;
    }
    if (argc == 2) {
        if (at_list[at_position].arg == 1) {
            return -1;
        }
        sprintf(buff_at, "%s\r", at_list[at_position].at_cmd);
        printf("%s\n", buff_at);
        flag_response = 1;
        uart_write(AT_UART, (uint8_t *)buff_at, strlen(buff_at) - 1);
        if (at_list[at_position].response) {
            xtimer_set_msg(&wait_timer, WAIT_RESPONSE, &msg_send, buff_thread_pid);
            if (xtimer_msg_receive_timeout(&msg_receive, WAIT_RESPONSE) < 0) { // dummy delay
                printf("\nTimer done\n");
            }
            flag_response = 0;
        } else {
            flag_response = 0;
        }
        return 0;
    }
    if (argc >= 3) {
        uint8_t diff = argc - 3;
        char aux_buf[32] = {0};
        for (int i = 0; i <= diff; i++) {
            strcat(aux_buf, argv[i + 2]);
            if (i != diff) {
                strcat(aux_buf, " ");
            }
        }
        if (at_list[at_position].arg == 0) {
            return -1;
        }
        sprintf(buff_at, "%s%s\r", at_list[at_position].at_cmd, aux_buf);
        printf("%s\n", buff_at);
        flag_response = 1;
        uart_write(AT_UART, (uint8_t *)buff_at, strlen(buff_at) - 1);
        if (at_list[at_position].response) {
            xtimer_set_msg(&wait_timer, WAIT_RESPONSE, &msg_send, buff_thread_pid);
            if (xtimer_msg_receive_timeout(&msg_receive, WAIT_RESPONSE) < 0) { // dummy delay
                printf("\nTimer done\n");
            }
            flag_response = 0;
        } else {
            flag_response = 0;
        }
        return 0;
    }
    return -1;
}

static const shell_command_t at_shell_commands[] = {{"at", "Use at help for information", cmd_at},
                                                    {NULL, NULL, NULL}};

int config_at(void) {

    int res = uart_init(AT_UART, AT_BAUDRATE, rx_cb, NULL);
    if (res != UART_OK) {
        printf("Error: Unable to initialize UART device\n");
        return res;
    }
    ringbuffer_init(&(uart_buffer.rx_buf), uart_buffer.rx_mem,
                    UART_BUFSIZE); // Initialize ringbuffer
    buff_thread_pid = thread_create(stack, sizeof(stack), PRIORITY, 0, buff_handler, NULL,
                                    "buff_handler"); // Start the buff_handler thread
    char line_buf[SHELL_BUFSIZE];
    shell_run(at_shell_commands, line_buf, SHELL_BUFSIZE); // Run the shell
    return 0;
}
