
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

#include <string.h>
#include <stdio.h>
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_system.h"
#include "wifi.h"
#include "storage.h"
#include "default_params.h"

static const char* TAG = "Wifi Module";

static int s_retry_num = 0;

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    //  STA EVENTS
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
    }

    // AP_EVENTS

    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",MAC2STR(event->mac), event->aid);
    }
}

esp_err_t init_sta (void)
{
    wifi_config_t sta_config;
    memset(&sta_config, 0, sizeof(wifi_config_t));
    memcpy(sta_config.sta.ssid, STA_WIFI_SSID, sizeof(STA_WIFI_SSID));
    memcpy(sta_config.sta.password, STA_WIFI_PASSWORD, sizeof(STA_WIFI_PASSWORD));

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    return ESP_OK;
}

int is_sta()
{
    wifi_mode_t mode;
    esp_err_t err = esp_wifi_get_mode(&mode);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_get_mode failed (%s)", esp_err_to_name(err));
        return 0;
    }

    if(mode == WIFI_MODE_STA){
        return 1;
    }

    return 0;
}

int is_ap()
{
    wifi_mode_t mode;
    esp_err_t err = esp_wifi_get_mode(&mode);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_get_mode failed (%s)", esp_err_to_name(err));
        return 0;
    }

   if(mode == WIFI_MODE_AP){
       return 1;
   }

  return 0;
}

esp_err_t init_ap (void)
{
    wifi_config_t wifi_config;
    esp_err_t err;

    memset(&wifi_config, 0 , sizeof(wifi_config_t));

    memcpy(&wifi_config.ap.ssid, AP_WIFI_SSID, sizeof(AP_WIFI_SSID));
    memcpy(&wifi_config.ap.password, AP_WIFI_PASSWORD, sizeof(AP_WIFI_PASSWORD));
    wifi_config.ap.ssid_len = strlen(AP_WIFI_SSID);
    wifi_config.ap.authmode = AP_AUTHMODE;
    wifi_config.ap.max_connection = AP_MAXCONN;
    wifi_config.ap.channel = AP_CHANNEL;

    if (strlen(AP_WIFI_PASSWORD) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    err = esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    if(err != ESP_OK){
        ESP_LOGE(TAG, "Error: to setting ap config");
        return err;
    }
    err = esp_wifi_start();
    if(err != ESP_OK){
        ESP_LOGE(TAG, "Error: to starting ap");
        return err;
    }

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             wifi_config.ap.ssid, wifi_config.ap.password, AP_CHANNEL);

    return ESP_OK;
}

uint8_t wifi_credentials_are_configured()
{
    uint8_t is_configured = 0;
    esp_err_t err = nvs_get_uint8(NVS_APP_NAMESPACE, NVS_IS_CONFIGURED_KEY,  &is_configured);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "it seem that wifi is not configured yet, it's will set the defaults credentials");
        return 0;
    }
    return is_configured;
}

esp_err_t set_default_credentials (void)
{
   esp_err_t err;
   err = nvs_set_string(NVS_APP_NAMESPACE, USER_AP_NAME_KEY, AP_WIFI_SSID);
   if(err != ESP_OK){
       ESP_LOGE(TAG, "Error: to setting AP SSID to the nvs");
   }

   err = nvs_set_string(NVS_APP_NAMESPACE, USER_AP_PASSWORD_KEY, AP_WIFI_PASSWORD);
   if(err != ESP_OK){
       ESP_LOGE(TAG, "Error: to setting AP Password to the nvs");
   }

   err = nvs_set_string(NVS_APP_NAMESPACE, USER_STA_NAME_KEY, STA_WIFI_SSID);
   if(err != ESP_OK){
       ESP_LOGE(TAG, "Error: to setting AP Password to the nvs");
   }

   err = nvs_set_string(NVS_APP_NAMESPACE, USER_STA_PASSWORD_KEY, STA_WIFI_PASSWORD);
   if(err != ESP_OK){
       ESP_LOGE(TAG, "Error: to setting AP Password to the nvs");
   }
    /* cppcheck-suppress duplicateCondition
    * (reason: <your reason why you think this is a false positive>) */
   if(err != ESP_OK){
       nvs_set_uint8(NVS_APP_NAMESPACE, NVS_IS_CONFIGURED_KEY, 0);
       return err;
   } else {
       nvs_set_uint8(NVS_APP_NAMESPACE, NVS_IS_CONFIGURED_KEY, 1);
       return ESP_OK;
   }

   return ESP_OK;
}

esp_err_t wifi_init(void)
{

    ESP_LOGI(TAG, "starting wifi module");

    if(nvs_init() != ESP_OK){
        ESP_LOGE(TAG, "Error: starting nvs");
    }

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    int is_configured = wifi_credentials_are_configured();

    if(!is_configured){
        set_default_credentials();
    }

    #if WIFI_ENABLE_STA && WIFI_ENABLE_AP
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(init_sta());
    ESP_ERROR_CHECK(init_ap());
    return ESP_OK;
    #endif

    #if CONFIG_ENABLE_AP
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(init_ap());
    #endif

    #if CONFIG_ENABLE_STA
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(init_sta());
    #endif

    return ESP_OK;
}
