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
 * @brief   Configure a https client to make a request
 *
 * @author  RocioRojas <RocioRojas391@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "esp_tls.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "httpsclient.h"

TaskHandle_t http_event_taks;

extern const char server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
extern const char server_root_cert_pem_end[] asm("_binary_server_root_cert_pem_end");

esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    static char *output_buffer;
    static int output_len;
    switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(__func__, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(__func__, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(__func__, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(__func__, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key,
                 evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(__func__, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        if (evt->user_data) {
            memcpy(evt->user_data + output_len, evt->data, evt->data_len);
        } else {
            if (output_buffer == NULL) {
                output_buffer = (char *)malloc(MAX_HTTP_OUTPUT_BUFFER);
                output_len = 0;
                if (output_buffer == NULL) {
                    ESP_LOGE(__func__, "Failed to allocate memory for output buffer");
                    return ESP_FAIL;
                }
            }
            memcpy(output_buffer + output_len, evt->data, evt->data_len);
        }
        output_len += evt->data_len;
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(__func__, "HTTP_EVENT_ON_FINISH");
        if (output_buffer != NULL) {
            free(output_buffer);
            output_buffer = NULL;
        }
        output_len = 0;
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(__func__, "HTTP_EVENT_DISCONNECTED");
        int mbedtls_err = 0;
        esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
        if (err != 0) {
            if (output_buffer != NULL) {
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            ESP_LOGI(__func__, "Last esp error code: 0x%x", err);
            ESP_LOGI(__func__, "Last mbedtls failure: 0x%x", mbedtls_err);
        }
        break;
    }
    return ESP_OK;
}

void request(void *http_params) {
    http_request_t *params = (http_request_t *)malloc(sizeof(http_request_t));
    *params = *(http_request_t *)http_params;
    uint8_t local_buff[MAX_HTTP_OUTPUT_BUFFER];
    esp_err_t err = ESP_FAIL;
    esp_http_client_handle_t client;
    esp_http_client_config_t config = {.url = params->url,
                                       .method = params->method,
                                       .cert_pem = server_root_cert_pem_start,
                                       .event_handler = _http_event_handler,
                                       .user_data = local_buff};
    http_response_t payload;
    switch (params->method) {
    case HTTP_METHOD_GET:
        client = esp_http_client_init(&config);
        err = esp_http_client_set_header(client, "content-type", params->content_type);
        if (err != ESP_OK) {
            ESP_LOGE(__func__, "Error Setting HTTP header, File: %s in %d", __FILE__, __LINE__);
        }
        err = esp_http_client_perform(client);
        if (err == ESP_OK) {
            int len = esp_http_client_get_content_length(client);
            payload.status = esp_http_client_get_status_code(client);
            payload.content_len = len;
            local_buff[len] = 0;
            memset(payload.output, 0, MAX_HTTP_OUTPUT_BUFFER);
            memcpy(payload.output, local_buff, len);
            response_callback_t callback = params->callback;
            callback(&payload);
            ESP_LOGI(__func__, "HTTPS Status = %d, content_length = %d",
                     esp_http_client_get_status_code(client),
                     esp_http_client_get_content_length(client));
        } else {
            ESP_LOGE(__func__, "Error perform HTTPS request %s", esp_err_to_name(err));
        }
        err = esp_http_client_cleanup(client);
        if (err != ESP_OK) {
            ESP_LOGE(__func__, "Error Closing HTTPS conennection, File: %s in %d", __FILE__,
                     __LINE__);
        }
        break;
    case HTTP_METHOD_POST:
        client = esp_http_client_init(&config);
        err = esp_http_client_set_header(client, "Content-Type", params->content_type);
        if (err != ESP_OK) {
            ESP_LOGE(__func__, "Error Setting HTTP header, File: %s in %d", __FILE__, __LINE__);
        }
        err = esp_http_client_set_post_field(client, params->body, strlen(params->body));
        if (err != ESP_OK) {
            ESP_LOGE(__func__, "Error Setting HTTP post field, File: %s in %d", __FILE__, __LINE__);
        }
        err = esp_http_client_perform(client);
        if (err == ESP_OK) {
            int len = esp_http_client_get_content_length(client);
            payload.status = esp_http_client_get_status_code(client);
            payload.content_len = len;
            local_buff[len] = 0;
            memset(payload.output, 0, MAX_HTTP_OUTPUT_BUFFER);
            memcpy(payload.output, local_buff, len);
            response_callback_t callback = params->callback;
            callback(&payload);
            ESP_LOGI(__func__, "HTTPS Status = %d, content_length = %d",
                     esp_http_client_get_status_code(client),
                     esp_http_client_get_content_length(client));
        } else {
            ESP_LOGE(__func__, "Error perform HTTPS request %s", esp_err_to_name(err));
        }
        err = esp_http_client_cleanup(client);
        if (err != ESP_OK) {
            ESP_LOGE(__func__, "Error Closing HTTPS conennection, File: %s in %d", __FILE__,
                     __LINE__);
        }
        break;
    default:
        break;
    }
    free(params);
    vTaskDelete(NULL);
}

void http_client(http_request_t *params) {
    xTaskCreate(&request, "http_test_task", 8192, params, 5, NULL);
}
