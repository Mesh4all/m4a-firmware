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
 * @brief   Wifi module
 *
 * @author  xkevin190 <kevinvelasco193@gmail.com>
 */
#include <stdio.h>
#include <string.h>
#include "ping/ping.h"
#include "ping/ping_sock.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "default_params.h"
#include "icmp_ping.h"

#define CONNECTED_BIT 1
#define FAIL_BIT      0

esp_ping_handle_t ping;
uint8_t is_connected = FAIL_BIT;
uint8_t is_configured = FAIL_BIT;
TaskHandle_t manual_ping_task;

void check_on_ping_success(esp_ping_handle_t hdl, void *args)
{
    uint8_t ttl;
    uint16_t seqno;
    uint32_t elapsed_time, recv_len;
    ip_addr_t target_addr;
    esp_ping_get_profile(hdl, ESP_PING_PROF_SEQNO, &seqno, sizeof(seqno));
    esp_ping_get_profile(hdl, ESP_PING_PROF_TTL, &ttl, sizeof(ttl));
    esp_ping_get_profile(hdl, ESP_PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    esp_ping_get_profile(hdl, ESP_PING_PROF_SIZE, &recv_len, sizeof(recv_len));
    esp_ping_get_profile(hdl, ESP_PING_PROF_TIMEGAP, &elapsed_time, sizeof(elapsed_time));
    ESP_LOGI(__func__,"PING %d bytes from %s icmp_seq=%d ttl=%d time=%d ms\n",
            (int)recv_len, inet_ntoa(target_addr.u_addr.ip4), seqno, ttl, (int)elapsed_time);
}

void check_on_ping_timeout(esp_ping_handle_t hdl, void *args)
{
    uint16_t seqno;
    ip_addr_t target_addr;
    esp_ping_get_profile(hdl, ESP_PING_PROF_SEQNO, &seqno, sizeof(seqno));
    esp_ping_get_profile(hdl, ESP_PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    printf("PING From %s icmp_seq=%d timeout\n", inet_ntoa(target_addr.u_addr.ip4), seqno);
}

void check_on_ping_end(esp_ping_handle_t hdl, void *args)
{
    uint32_t transmitted;
    uint32_t received;
    uint32_t total_time_ms;

    esp_ping_get_profile(hdl, ESP_PING_PROF_REQUEST, &transmitted, sizeof(transmitted));
    esp_ping_get_profile(hdl, ESP_PING_PROF_REPLY, &received, sizeof(received));
    esp_ping_get_profile(hdl, ESP_PING_PROF_DURATION, &total_time_ms, sizeof(total_time_ms));
    ESP_LOGI(__func__,"PING %d packets transmitted,%d received, time %dms\n",
                      (int)transmitted,(int)received, (int)total_time_ms);

    if (received > 0) {
        is_connected = CONNECTED_BIT;
    }
    else {
        is_connected = FAIL_BIT;
    }
}

esp_err_t initialize_ping()
{
    ip_addr_t target_addr;
    struct addrinfo hint;
    struct addrinfo *res = NULL;
    memset(&hint, 0, sizeof(hint));
    memset(&target_addr, 0, sizeof(target_addr));
    int result = getaddrinfo(DEFAULT_SERVER_TO_PING, NULL, &hint, &res);
    if((result != 0) || (res == NULL)) {
        is_connected = FAIL_BIT;
        is_configured = FAIL_BIT;
        ESP_LOGE(__func__,"DNS lookup failed err=%d res=%p", result, res);
        return ESP_FAIL;
    } else {
        is_configured = CONNECTED_BIT;
        printf("Initialising ping");
        struct in_addr addr4  =  ((struct sockaddr_in *) (res->ai_addr))->sin_addr;
        inet_addr_to_ip4addr(ip_2_ip4(&target_addr), &addr4);
        freeaddrinfo(res);
        esp_ping_config_t ping_config = ESP_PING_DEFAULT_CONFIG();
        ping_config.target_addr = target_addr;   // target IP address
        ping_config.count = 1;    // ping in infinite mode, esp_ping_stop can stop it

        /* set callback functions */
        esp_ping_callbacks_t cbs;
        cbs.on_ping_success = check_on_ping_success;
        cbs.on_ping_timeout = check_on_ping_timeout;
        cbs.on_ping_end = check_on_ping_end;
        cbs.cb_args = NULL;

        esp_err_t err = esp_ping_new_session(&ping_config, &cbs, &ping);
        if (err != ESP_OK) {
            ESP_LOGE(__func__,
                    "Error to the init ping session %s",
                    esp_err_to_name(err));
            return err;
        }

        return ESP_OK;
    }
}

void send_ping (void *arg)
{
    while (1)
    {
        if(!is_configured) {
            initialize_ping();
        }
        esp_ping_start(ping);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

uint8_t get_current_status (void)
{
    return is_connected;
}

void ping_task (void *arg)
{
    callback_t callback = (callback_t*)arg;
    esp_ping_start(ping);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    callback(is_connected);
    vTaskDelete(manual_ping_task);
}

void manual_ping (callback_t* callback)
{
    xTaskCreate(ping_task, "manual_ping", 1024*2, callback, 4, &manual_ping_task);
}

esp_err_t enable_ping(void)
{
    esp_err_t err = initialize_ping();
    if(err != ESP_OK) {
        ESP_LOGI(__func__,"Error to the init ping %s", esp_err_to_name(err));
        return err;
    }
    xTaskCreate(send_ping, "ping_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
    return ESP_OK;
}
