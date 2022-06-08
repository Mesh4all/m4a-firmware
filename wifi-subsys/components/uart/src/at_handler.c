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
 *
 * @author  xkevin190 <kevinvelasco193@gmail.com>
 */

#include <stdio.h>
#include <string.h>

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "subsys_uart.h"

#include "at_handler.h"
#include "subsys_uart.h"
#include "wifi.h"

#define ARRAY_SIZE 11

char *default_keys[ARRAY_SIZE] = {
    "NVS_RST", "WIFI_OFF",  "WAP_SSID", "WAP_PASS",  "WAP_CHAN",  "WAP_AUTH",
    "WIFI_ON", "WIFI_MODE", "WIFI_RST", "WSTA_SSID", "WSTA_PASS",
};

char *symbols[4] = {
    "+",
    "&",
    "?",
    "=",
};

int get_symbol(char *command, size_t len, char *output) {
    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < 4; j++) {
            if (command[i] == symbols[j][0]) {
                memcpy(output, symbols[j], 2);
                return ESP_OK;
            }
        }
    }
    return ESP_FAIL;
}

esp_err_t parse_at_message(uint8_t *at_command, at_request_t *output) {
    size_t at_size = strlen((char *)at_command) + 1;
    char commands[at_size];
    memcpy(commands, at_command, at_size);
    char plus[2];
    memset(plus, "0", sizeof(plus));
    get_symbol(commands, at_size, plus);
    char *key = strtok(commands, plus);
    char first_step[100];
    char *key_value = NULL;
    char *value = NULL;

    int i = 0;
    char second_character[2];
    while (key != NULL) {
        if (i == 1) {
            memcpy(first_step, key, sizeof(first_step));
        }
        i++;

        key = strtok(NULL, plus);
    }

    get_symbol(first_step, at_size, second_character);
    char *key2 = strtok(first_step, second_character);
    i = 0;
    while (key2 != NULL) {
        if (i == 0) {
            key_value = (char *)malloc(strlen(key2) + 1);

            memset(key_value, 0, strlen(key2) + 1);
            memcpy(key_value, key2, strlen(key2) + 1);
        }
        if (i == 1) {
            value = (char *)malloc(strlen(key2) + 1);
            memcpy(value, key2, strlen(key2) + 1);
        }
        i++;
        key2 = strtok(NULL, "+");
    }

    /* cppcheck-suppress nullPointerRedundantCheck
     * (reason: <is necessary verify that this data is not null>) */
    memcpy(output->key, key_value, strlen(key_value) + 1);
    if (value != NULL) {
        memcpy(output->value, value, strlen(value) + 1);
    }

    memcpy(output->action, plus, 2);
    memcpy(output->read_or_write, second_character, 2);

    if (key_value == NULL && value == NULL) {
        return ESP_FAIL;
    }

    free(value);
    free(key_value);

    return ESP_OK;
}

int at_cmd_to_name(char *key) {
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        key[strlen(key)] = 0;
        if (strcmp(key, default_keys[i]) == 0) {
            return i;
        }
    }

    return -1;
}

void action_request(enum at_keys_n at_key, at_request_t *payload) {
    esp_err_t err = ESP_FAIL;
    switch (at_key) {
    case (NVS_RST):
        err = wifi_restore_default();
        break;
    case (WIFI_OFF):
        err = wifi_turn_off();
        break;
    case (WIFI_ON):
        err = wifi_on();
        break;
    case (WIFI_RST):
        err = wifi_restart();
        break;

    default:
        ESP_LOGE(__func__, "unhandler event %i \n", at_key);
        err = ESP_FAIL;
        break;
    }

    if (err != ESP_OK) {
        sendData(__func__, "\r\n400\r\n");
    } else {
        sendData(__func__, "\r\n200\r\n");
    }
}

void write_request(enum at_keys_n at_key, at_request_t *payload) {
    int wifi_mode = 0;
    uint8_t channel = 0;
    uint8_t max_connection = 0;
    uint8_t auth = 0;
    esp_err_t err = ESP_FAIL;
    if (strcmp(payload->read_or_write, symbols[2]) == 0) {
        ESP_LOGI(__func__, "read AT request");

        err = ESP_OK;
        // read AT request is not defined yet.
    } else if (strcmp(payload->read_or_write, symbols[3]) == 0) {
        switch (at_key) {
        case (WAP_SSID):
            err = change_wifi_ap_ssid(payload->value);
            break;
        case (WAP_PASS):
            err = change_wifi_ap_pass(payload->value);
            break;
        case (WAP_CHAN):
            channel = atoi(payload->value);
            err = select_wifi_channel(channel);
            break;
        case (WAP_AUTH):
            auth = atoi(payload->value);
            err = change_ap_auth(auth);
            break;
        case (WIFI_MODE):
            wifi_mode = atoi(payload->value);
            err = change_wifi_mode(wifi_mode);
            break;
        case (WSTA_SSID):
            err = change_wifi_sta_ssid(payload->value);
            break;
        case (WSTA_PASS):
            err = change_wifi_sta_pass(payload->value);
            break;
        default:
            ESP_LOGE(__func__, "unhandler event %i \n", at_key);
            err = ESP_FAIL;
            break;
        }
    }

    if (err != ESP_OK) {
        sendData(__func__, "\r\n400\r\n");
    } else {
        sendData(__func__, "\r\n200\r\n");
    }
}

void at_handler(uint8_t *at_command) {
    ESP_LOGI(__func__, "enter in at handler");
    at_request_t payload;

    payload.key = (char *)malloc(32);
    payload.value = (char *)malloc(32);

    parse_at_message(at_command, &payload);

    enum at_keys_n at_key = at_cmd_to_name(payload.key);
    ESP_LOGI(__func__, "the key is %d the string key is %s and %s and %s and %s", at_key,
             payload.key, payload.value, payload.action, payload.read_or_write);

    if (strcmp(payload.action, symbols[0]) == 0) {
        write_request(at_key, &payload);
    } else if (strcmp(payload.action, symbols[1]) == 0) {
        action_request(at_key, &payload);
    } else {
        sendData(__func__, "\r\n400\r\n");
    }
}
