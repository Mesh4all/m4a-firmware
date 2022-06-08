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
 * @brief   ICMP protocol component
 *
 * @author  xkevin190 <kevinvelasco193@gmail.com>
 */
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "lwip/dns.h"
#include "lwip/err.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "ping/ping.h"
#include "ping/ping_sock.h"

#include "default_params.h"
#include "icmp_ping.h"
#include "storage.h"

#define CONNECTED_BIT 1
#define FAIL_BIT 0

esp_ping_handle_t ping;
uint8_t is_connected = FAIL_BIT;
uint8_t is_configured = FAIL_BIT;
TaskHandle_t manual_ping_task;
EventGroupHandle_t s_icmp_event_group;

void check_on_ping_success(esp_ping_handle_t hdl, void *args) {
    uint8_t ttl;
    uint16_t seqno;
    uint32_t elapsed_time, recv_len;
    ip_addr_t target_addr;
    esp_ping_get_profile(hdl, ESP_PING_PROF_SEQNO, &seqno, sizeof(seqno));
    esp_ping_get_profile(hdl, ESP_PING_PROF_TTL, &ttl, sizeof(ttl));
    esp_ping_get_profile(hdl, ESP_PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    esp_ping_get_profile(hdl, ESP_PING_PROF_SIZE, &recv_len, sizeof(recv_len));
    esp_ping_get_profile(hdl, ESP_PING_PROF_TIMEGAP, &elapsed_time, sizeof(elapsed_time));
    ESP_LOGI(__func__, "PING %d bytes from %s icmp_seq=%d ttl=%d time=%d ms\n", (int)recv_len,
             inet_ntoa(target_addr.u_addr.ip4), seqno, ttl, (int)elapsed_time);
}

void check_on_ping_timeout(esp_ping_handle_t hdl, void *args) {
    uint16_t seqno;
    ip_addr_t target_addr;
    esp_ping_get_profile(hdl, ESP_PING_PROF_SEQNO, &seqno, sizeof(seqno));
    esp_ping_get_profile(hdl, ESP_PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    printf("PING From %s icmp_seq=%d timeout\n", inet_ntoa(target_addr.u_addr.ip4), seqno);
}
void check_on_ping_end(esp_ping_handle_t hdl, void *args) {
    uint32_t transmitted;
    uint32_t received;
    uint32_t total_time_ms;

    esp_ping_get_profile(hdl, ESP_PING_PROF_REQUEST, &transmitted, sizeof(transmitted));
    esp_ping_get_profile(hdl, ESP_PING_PROF_REPLY, &received, sizeof(received));
    esp_ping_get_profile(hdl, ESP_PING_PROF_DURATION, &total_time_ms, sizeof(total_time_ms));
    ESP_LOGI(__func__, "PING %d packets transmitted, %d received, time %dms\n", (int)transmitted,
             (int)received, (int)total_time_ms);
    if (received > 0) {
        is_connected = CONNECTED_BIT;
        xEventGroupSetBits(s_icmp_event_group, CONNECTED_BIT);
    } else {
        is_connected = FAIL_BIT;
        xEventGroupSetBits(s_icmp_event_group, FAIL_BIT);
    }
}

esp_err_t initialize_ping() {
    ESP_LOGI(__func__, "Starting ping setup");
    ip_addr_t target_addr;
    struct addrinfo hint;
    struct addrinfo *res = NULL;
    char url[50];
    size_t url_len = 0;
    memset(&hint, 0, sizeof(hint));
    memset(&target_addr, 0, sizeof(target_addr));
    esp_err_t err = nvs_get_string(stringlify(PING), stringlify(PING_TO), url, &url_len);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "error to get url the cause is %s", esp_err_to_name(err));

        return err;
    }
    int result = getaddrinfo(url, NULL, &hint, &res);
    if ((result != 0) || (res == NULL)) {
        is_connected = FAIL_BIT;
        is_configured = FAIL_BIT;
        ESP_LOGE(__func__, "DNS lookup failed err=%d res=%p", result, res);
        return ESP_FAIL;
    } else {
        is_configured = CONNECTED_BIT;
        struct in_addr addr4 = ((struct sockaddr_in *)(res->ai_addr))->sin_addr;
        inet_addr_to_ip4addr(ip_2_ip4(&target_addr), &addr4);
        freeaddrinfo(res);
        esp_ping_config_t ping_config = ESP_PING_DEFAULT_CONFIG();
        ping_config.target_addr = target_addr; // target IP address
        err =
            nvs_get_uint32(stringlify(PING), stringlify(PING_RET),
                           &ping_config.count); // ping in infinite mode, esp_ping_stop can stop it
        if (err != ESP_OK) {
            ESP_LOGE(__func__, "error to get ping count the cause is %s", esp_err_to_name(err));

            return err;
        }
        err = nvs_get_uint32(stringlify(PING), stringlify(PING_INT),
                             &ping_config.interval_ms); // Milliseconds between each ping procedure
        if (err != ESP_OK) {
            ESP_LOGE(__func__, "error to get ping time the cause is %s", esp_err_to_name(err));

            return err;
        }
        /* set callback functions */
        esp_ping_callbacks_t cbs;
        cbs.on_ping_success = check_on_ping_success;
        cbs.on_ping_timeout = check_on_ping_timeout;
        cbs.on_ping_end = check_on_ping_end;
        cbs.cb_args = NULL;

        err = esp_ping_new_session(&ping_config, &cbs, &ping);
        if (err != ESP_OK) {
            ESP_LOGE(__func__, "Error to the init ping session %s", esp_err_to_name(err));
            return err;
        }

        return ESP_OK;
    }
}

void send_ping(void *arg) {
    uint32_t ping_timeout = 0;
    esp_err_t err = nvs_get_uint32(stringlify(PING), stringlify(PING_TIMEOUT), &ping_timeout);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "Error to the timeout ping session %s", esp_err_to_name(err));
    } else {
        while (1) {
            if (!is_configured) {
                initialize_ping();
            }
            esp_ping_start(ping);
            vTaskDelay(ping_timeout / portTICK_PERIOD_MS);
        }
    }
}

uint8_t get_current_status(void) { return is_connected; }

void ping_task(void *arg) {
    s_icmp_event_group = xEventGroupCreate();
    uint32_t milliseconds = 0;
    callback_t callback = (callback_t)arg;
    esp_ping_start(ping);
    esp_err_t err = nvs_get_uint32(stringlify(PING), stringlify(PING_INT),
                                   &milliseconds); // Milliseconds between each ping procedure
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "error to get ping time the cause is %s", esp_err_to_name(err));
    } else {
        xEventGroupWaitBits(s_icmp_event_group, CONNECTED_BIT | FAIL_BIT, pdFALSE, pdFALSE,
                            portMAX_DELAY);

        callback(is_connected);
        vTaskDelete(manual_ping_task);
        vEventGroupDelete(s_icmp_event_group);
    }
}

void manual_ping(callback_t callback) {
    xTaskCreate(ping_task, "manual_ping", 1024 * 2, callback, 4, &manual_ping_task);
}

esp_err_t enable_ping(void) {
    esp_err_t err = initialize_ping();
    if (err != ESP_OK) {
        ESP_LOGI(__func__, "Error to the init ping %s", esp_err_to_name(err));
        return err;
    }
    xTaskCreate(send_ping, "ping_task", 1024 * 2, NULL, 2, NULL);
    return ESP_OK;
}
