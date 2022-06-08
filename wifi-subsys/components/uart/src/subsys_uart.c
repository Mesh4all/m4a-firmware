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
 * @brief  Uart handler
 *
 * @author xkevin190 <kevinvelasco193@gmail.com>
 */

#include <string.h>
#include <cJSON.h>

#include "subsys_uart.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "at_handler.h"
#include "wifi.h"
#include "default_params.h"
#include "serialization.h"
#include "httpsclient.h"

static const int RX_BUF_SIZE = 1024;
static uint8_t at_mode = 0;

#define TXD_PIN (GPIO_NUM_10)
#define RXD_PIN (GPIO_NUM_9)

char *COMMAND[11] = {"AT+WAP_SSID=new_ssid",
                     "AT+WAP_PASS=new_password",
                     "AT+WSTA_SSID=new_ssid",
                     "AT+WSTA_PASS=new_passoword",
                     "AT+WIFI_OFF",
                     "AT+WIFI_ON",
                     "AT+NVS_RST",
                     "AT+WIFI_MODE=1",
                     "AT+WAP_CHAN=1",
                     "AT+WAP_AUTH=1",
                     "AT+WIFI_RST"};

esp_err_t setting_uart(void) {
    esp_err_t err;
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // We won't use a buffer for sending data.
    err = uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    if (err != ESP_OK) {
        return err;
    }
    err = uart_param_config(UART_NUM_1, &uart_config);
    if (err != ESP_OK) {
        return err;
    }
    err = uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (err != ESP_OK) {
        return err;
    }

    return ESP_OK;
}

void callback(http_response_t *response) {
    ESP_LOGI(__func__, "executed callback the response: %d !!! %s", response->status,
             response->output);
}

int sendData(const char *logName, char *data) {
    const int len = strlen(data);
    printf("before send %s \n", data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

void received_sensor_data(uint8_t *values, size_t len) {
    char *message = (char *)values;

    int val_received = 0;
    ESP_LOGI(__func__, "Read sensor data: '%s' and len is: %d ", message, len);
    decode(values, "temp", &val_received, len);
    ESP_LOGI(__func__, "%d ", val_received);

    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temp", val_received);
    char *payload = cJSON_Print(root);

    http_request_t request;
    request.method = HTTP_METHOD_POST;
    request.content_type = HTTPS_CONTENT_JSON;
    memcpy(request.url, "https://cloud.mesh4all.org", sizeof("https://cloud.mesh4all.org"));
    memcpy(request.body, payload, strlen(payload) + 1);
    request.callback = &callback;
    http_client(&request);
}

void tx_send_loop(void *arg) {
    static const char *TX_TASK_TAG = "TX_TASK";
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
    int counter = 0;
    while (1) {
        if (counter == 4) {
            counter = 0;
        } else {
            counter = counter + 1;
        }
        sendData(TX_TASK_TAG, COMMAND[counter]);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void rx_receive(void *arg) {
    esp_log_level_set(__func__, ESP_LOG_INFO);
    uint8_t *data = (uint8_t *)malloc(RX_BUF_SIZE + 1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            if (strcmp((char *)data, stringlify(ATH)) == 0) {
                ESP_LOGI(__func__, "starting at mode");
                at_mode = 1;
            } else if ((strcmp((char *)data, stringlify(AT)) == 0) && at_mode == 1) {
                ESP_LOGI(__func__, "ESP has connection");
                sendData(__func__, "\r\nok\r\n");
            } else if (strcmp((char *)data, stringlify(ATHO)) == 0) {
                ESP_LOGI(__func__, "leaving of at mode");
                at_mode = 0;
            } else if (at_mode) {
                if (data[0] == 0x41 && data[1] == 0x54) {
                    at_handler(data);
                } else {
                    ESP_LOGI(__func__, "the command must init with AT");
                }
            } else {
                received_sensor_data(data, rxBytes);
            }

            ESP_LOG_BUFFER_HEXDUMP(__func__, data, rxBytes, ESP_LOG_INFO);
        }
    }
    free(data);
}

esp_err_t init_uart() {
    esp_err_t err = setting_uart();
    if (err != ESP_OK) {
        return err;
    }
    xTaskCreate(rx_receive, "uart_rx_task", 1024 * 2, NULL, configMAX_PRIORITIES, NULL);
    return ESP_OK;
}
