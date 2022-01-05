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
 * @file
 * @brief       this is an example of uart
 * @author      xkevin190 <kevinvelasco190@gmail.com>
 */

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>

#include "board.h"
#include "shell.h"
#include "periph/uart.h"
#include "msg.h"
#include "ringbuffer.h"
#include "xtimer.h"
#include "ztimer.h"
#include "at.h"
#include "periph/adc.h"
#include "log.h"

#define POWEROFF_DELAY      (250U * US_PER_MS)
#define RES             ADC_RES_10BIT
#define DELAY_MS        5000U

#define UART_BLE UART_DEV(1)

#define GPS_BAUDRATE 9600
#define MAX_SENTENCE_SIZE 128
#define MAIN_QUEUE_SIZE (8)
#define UART_BUFSIZE        (128U)
msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

size_t count = 0;
char uart_buff[MAX_SENTENCE_SIZE] = {0};

typedef struct {
    char rx_mem[UART_BUFSIZE];
    ringbuffer_t rx_buf;
} uart_ctx_t;
static uart_ctx_t ctx[UART_NUMOF];

int dummy_cmd(int argc, char **argv)
{
    return 0;
}

void init_adc(void) {
   if (adc_init(0) < 0) {
        printf("Initialization of ADC_LINE failed\n");
    } else {
        printf("Successfully initialized ADC_LINE\n");
    }
    while (1) {
            int sample = adc_sample(0, RES);
            double voltage = ((sample * 1100)/1024)/10;
            if (sample < 0) {
                printf("ADC_LINE: selected resolution not applicable\n");
            } else {
                LOG_INFO("sample %i \n", sample);
                LOG_INFO("voltage %i \n", (int)voltage);
            }
        ztimer_sleep(ZTIMER_MSEC, DELAY_MS);
    }
}

int init_at_mode (int argc, char **argv){
    const char test_string[] = "AT";
    uart_write(UART_BLE, (uint8_t*)test_string, sizeof(test_string));
    return 0;
}

int send_uart_reset (int argc, char **argv) {
    const char test_string[] = "AT+RESET";
    uart_write(UART_BLE, (uint8_t*)test_string, sizeof(test_string));
    return 0;
}

int send_uart_message (int argc, char **argv) {
    uart_write(UART_BLE, (uint8_t*)argv[1], strlen((char*)argv[1]));
    return 0;
}

int send_uart_password (int argc, char **argv) {
    uart_write(UART_BLE, (uint8_t*)argv[1], strlen((char*)argv[1]));
    return 0;
}

static const shell_command_t shell_commands[] = {
    { "udp", "send udp packets", dummy_cmd },
    {"send", "send packets since uart", send_uart_message},
    {"init", "init at mode", init_at_mode},
    {"reset", "reset hc-06", send_uart_reset},
    {"password", "add password", send_uart_password},
    { NULL, NULL, NULL }
};

static void gps_rx_cb(void *arg, uint8_t c){
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

int main (void)
{
    if(uart_init(UART_BLE, GPS_BAUDRATE, gps_rx_cb, NULL) < 0){
        printf("ERROR uart \n");
    } else {
        printf("Success: Initialized UART_DEV(%i) at BAUD %"PRIu32"\n", UART_BLE, GPS_BAUDRATE);
    }
    init_adc();
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
}
