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
 * @brief       Main firmware file
 *
 * @author      luisan00 <luisan00@hotmail.com>
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "msg.h"
#include "log.h"
#include "board.h"
#include "xtimer.h"

#include "rpl_protocol.h"
#include "ds18_sensor.h"
#include "serialization.h"
#include "uniqueid.h"
#include "udp_client.h"
#include "udp_server.h"
#include "at_client.h"
#include "periph/uart.h"
#include "board.h"

#define MAIN_QUEUE_SIZE (8)
#define UART_PORT UART_DEV(1)
#define ADDRESS_TO_SEND "fe80::204:2519:1801:cad4"

msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
char get_sensor_event[THREAD_STACKSIZE_DEFAULT];
uint8_t buffer[100];
shell_command_t shell_extended_commands[] = {{NULL, NULL, NULL}};

void *init_loop(void *args) {
    (void)args;
    sensor_data payload;
    uint8_t addr_cpu[CPUID_LEN] = {0};
    CPUID(addr_cpu);
    memcpy(payload.id_node, addr_cpu, CPUID_LEN);
    int port = 3000;
    while (1) {
        if (get_temperature_value(&payload.temperature) < 0) {
            payload.temperature = 0;
            printf("Error to the obtain the temperature \n");
        }
        printf("%d,%d °C\n", payload.temperature / 100, payload.temperature % 100);
        size_t len_cbor = sizeof(sensor_data);
        if (cbor_enconde_message(&payload, buffer, &len_cbor) < 0) {
            printf("error to encode payload with cbor");
        } else {
            udp_send(port, ADDRESS_TO_SEND, buffer, &len_cbor);
        }
        xtimer_sleep(60);
    }
}

void udp_callback(uint8_t *message, size_t len) {
    sensor_data output;
    cbor_decode_message(message, &output, len);
    uart_write(UART_PORT, (uint8_t *)message, len);
}

int main(void) {
    LOG_INFO("~~ Welcome to System exam ~~\n");
    /* Start shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    rpl_setup(CONFIG_IS_DODAG);

#if CONFIG_IS_DODAG == 0

    int8_t err = init_temperature_sensor(17);
    if (err != 0) {
        printf("Error: temperature sensor not inizializated");
        return err;
    }
    if (thread_create(get_sensor_event, sizeof(get_sensor_event), THREAD_PRIORITY_MAIN - 1,
                      THREAD_CREATE_STACKTEST, *init_loop, NULL,
                      "Sensors event") <= KERNEL_PID_UNDEF) {
        printf("peripherals event loop could not init \n");
        return -1;
    }
#else
    udpf_payload payload;
    payload.port = 3000;
    payload.callback = &udp_callback;
    udp_server(&payload);
    config_at();
#endif

    shell_run(shell_extended_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
