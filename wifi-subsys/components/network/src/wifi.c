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

static int s_retry_num = 0;

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    //  STA EVENTS
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < WSTA_RETRIES)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(__func__, "retry to connect to the AP");
        }
        ESP_LOGI(__func__, "connect to the AP fail");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(__func__, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
    }

    // AP_EVENTS
    if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(__func__, "station " MACSTR " join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(__func__, "station " MACSTR " leave, AID=%d", MAC2STR(event->mac), event->aid);
    }
}

esp_err_t init_sta(void)
{
    esp_err_t err;
    wifi_config_t sta_config;
    memset(&sta_config, 0, sizeof(wifi_config_t));
    size_t ssid_len = 0;
    size_t password_len = 0;

    err = nvs_get_string(stringlify(WSTA),
                        stringlify(WSTA_SSID),
                        &sta_config.sta.ssid,
                        &ssid_len);

    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                 "Error to the get sta password (%s)",
                 esp_err_to_name(err));
        return err;
    }

    err = nvs_get_string(stringlify(WSTA),
                        stringlify(WSTA_PASS),
                        &sta_config.sta.password,
                        &password_len);

    if(err != ESP_OK) {
         ESP_LOGE(__func__,
                 "Error to the get sta password (%s)",
                 esp_err_to_name(err));
        return err;
    }

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    return ESP_OK;
}

int is_sta()
{
    wifi_mode_t mode;
    esp_err_t err = esp_wifi_get_mode(&mode);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "esp_wifi_get_mode failed (%s)", esp_err_to_name(err));
        return 0;
    }

    if (mode == WIFI_MODE_STA)
    {
        return 1;
    }

    return 0;
}

int is_ap()
{
    wifi_mode_t mode;
    esp_err_t err = esp_wifi_get_mode(&mode);
    if (err != ESP_OK) {
        ESP_LOGE(__func__, "esp_wifi_get_mode failed (%s)", esp_err_to_name(err));
        return 0;
    }

    if (mode == WIFI_MODE_AP) {
        return 1;
    }

    return 0;
}

esp_err_t init_ap(void)
{
    wifi_config_t wifi_config;
    esp_err_t err;
    size_t ssid_len = 0;
    size_t password_len = 0;

    memset(&wifi_config, 0, sizeof(wifi_config_t));

    err = nvs_get_string(stringlify(WAP),
                        stringlify(WAP_SSID),
                        &wifi_config.ap.ssid,
                        &ssid_len);

    if (err != ESP_OK) {
        ESP_LOGE(__func__, "error to get WAP SSID the cause is %s", esp_err_to_name(err));
        return err;
    }

    err = nvs_get_string(stringlify(WAP),
                        stringlify(WAP_PASS),
                        &wifi_config.sta.password,
                        &password_len);

    if(err != ESP_OK) {
        ESP_LOGE(__func__,
                "error to get WAP PASSWORD the cause is %s",
                esp_err_to_name(err));
        return err;
    }

    err = nvs_get_uint8(stringlify(WAP),
                        stringlify(WAP_MAXCON),
                        &wifi_config.ap.max_connection);
    if(err != ESP_OK) {
        ESP_LOGE(__func__,
                "error to get max_connections the cause is %s",
                esp_err_to_name(err));

        return err;
    }

    err = nvs_get_uint8(stringlify(WAP),
                          stringlify(WAP_CHAN),
                        &wifi_config.ap.channel);
    if(err != ESP_OK) {
        ESP_LOGE(__func__,
                "error to get AP channel the cause is %s",
                esp_err_to_name(err));
        return err;
    }

    wifi_config.ap.ssid_len = ssid_len;

    if (password_len == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }
    else {
        err = nvs_get_uint8(stringlify(WAP),
                        stringlify(WAP_AUTH),
                        &wifi_config.ap.authmode);

        if(err != ESP_OK) {
            ESP_LOGE(__func__,
                "error to get ap auth mode the cause is %s",
                esp_err_to_name(err));
            return err;
        }
    }

    err = esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "error to the set wifi mode the cause is %s",
                esp_err_to_name(err));
        return err;
    }
    err = esp_wifi_start();

    if (err != ESP_OK) {
         ESP_LOGE(__func__,
                "error to start the wifi, the cause is %s",
                esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(__func__, "WiFi SOFTAP finished. SSID:%s password:%s channel:%d",
             wifi_config.ap.ssid, wifi_config.ap.password, wifi_config.ap.channel);

    return ESP_OK;
}

uint8_t wifi_credentials_are_configured()
{
    uint8_t is_configured = 0;

    esp_err_t err = nvs_get_uint8(stringlify(NVS),
                                  stringlify(NVS_IS_INIT),
                                  &is_configured);

    if (err != ESP_OK)
    {
        ESP_LOGE(__func__, "It seems that WiFi is not configured yet, setting defaults parameters");
        return 0;
    }

    return is_configured;
}

int identify_enabled_interface (void)
{
#if WSTA_ENABLED && WAP_ENABLED
    return WIFI_MODE_APSTA;
#elif WAP_ENABLED
    return WIFI_MODE_AP;
#elif WSTA_ENABLED
    return WIFI_MODE_STA;
#else
    return 0;
#endif
}

esp_err_t set_default_credentials(void)
{
    esp_err_t err = nvs_set_string(stringlify(WAP), stringlify(WAP_SSID), WAP_SSID);
    if (err != ESP_OK)
    {
        ESP_LOGE(__func__,
                "Error: to setting AP SSID to the nvs %s",
                esp_err_to_name(err));
    }

    err = nvs_set_string(stringlify(WAP), stringlify(WAP_PASS), WAP_PASS);
    if (err != ESP_OK)
    {
        ESP_LOGE(__func__,
                "Error: to setting AP Password to the nvs %s",
                esp_err_to_name(err));
    }

    err = nvs_set_string(stringlify(WSTA), stringlify(WSTA_SSID), WSTA_SSID);
    if (err != ESP_OK)
    {
        ESP_LOGE(__func__,
                "Error: to setting STA ssid to the nvs %s",
                esp_err_to_name(err));
    }

    err = nvs_set_string(stringlify(WSTA), stringlify(WSTA_PASS), WSTA_PASS);
    if (err != ESP_OK)
    {
        ESP_LOGE(__func__,
                "Error: to setting STA password to the nvs %s",
                esp_err_to_name(err));
    }

    err = nvs_set_uint8(stringlify(WAP), stringlify(WAP_CHAN), WAP_CHAN);
    if (err != ESP_OK)
    {
        ESP_LOGE(__func__,
                "Error: to setting WAP channel to the nvs %s",
                esp_err_to_name(err));
    }

    err = nvs_set_uint8(stringlify(WAP), stringlify(WAP_MAXCON), WAP_MAXCON);
    if (err != ESP_OK)
    {
        ESP_LOGE(__func__,
                "Error: to setting AP max connections to the nvs %s",
                esp_err_to_name(err));
    }

    err = nvs_set_uint8(stringlify(WAP), stringlify(WAP_AUTH),WAP_AUTH);
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to setting AP auth to the nvs %s",
                esp_err_to_name(err));
    }
    err = nvs_set_uint8(stringlify(WIFI),
                        stringlify(WIFI_MODE),
                        identify_enabled_interface());
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to setting WIFI mode to the nvs %s",
                esp_err_to_name(err));
    }
    /* cppcheck-suppress duplicateCondition
    * (reason: <is necessary because all function executed before,
    needed response ESP_OK for save the nvs configuration in true>) */
    if (err != ESP_OK) {
        nvs_set_uint8(stringlify(NVS), stringlify(NVS_IS_INIT), 0);
        return err;
    }
    else {
        nvs_set_uint8(stringlify(NVS), stringlify(NVS_IS_INIT), 1);
        return ESP_OK;
    }

    return ESP_OK;
}

esp_err_t wifi_on(void)
{
  esp_err_t err =  wifi_init();
  if (err != ESP_OK) {
      ESP_LOGE(__func__,
                "Error: to the init wifi %s",
                esp_err_to_name(err));
      return err;
  }
  return ESP_OK;
}

esp_err_t wifi_off(void)
{
   esp_err_t err = esp_wifi_stop();
   if(err != ESP_OK) {
          ESP_LOGE(__func__,
                "Error: to  the turn off the wifi %s",
                esp_err_to_name(err));
       return err;
   }
   return ESP_OK;
}

esp_err_t wifi_restart(void)
{
    esp_err_t err;
    err = wifi_off();
    if(err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to the turn off the wifi %s",
                esp_err_to_name(err));
        return err;
    }
    err = wifi_on();
    if(err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to start the wifi %s",
                esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}

esp_err_t change_wifi_sta_ssid (char* sta_ssid)
{
    esp_err_t err;
    err = nvs_set_string(stringlify(WSTA), stringlify(WSTA_SSID), sta_ssid);
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to settin STA ssid to the nvs %s",
                esp_err_to_name(err));
    }
    else {
        err = init_sta();
        if (err != ESP_OK) {
            ESP_LOGE(__func__,
                "Error: to the init the wifi %s",
                esp_err_to_name(err));
           return err;
        }
    }
    return ESP_OK;
}

esp_err_t change_wifi_ap_ssid (char* ap_ssid)
{
    esp_err_t err;
    err = nvs_set_string(stringlify(WAP), stringlify(WAP_SSID), ap_ssid);
    if (err != ESP_OK) {
           ESP_LOGE(__func__,
                "Error: to settin STA ssid to the nvs %s",
                esp_err_to_name(err));
    }
    else {
       err = init_ap();
       if (err != ESP_OK) {
            ESP_LOGE(__func__,
                "Error: to the init the wifi %s",
                esp_err_to_name(err));
           return err;
       }
    }
    return ESP_OK;
}

esp_err_t change_wifi_ap_pass (char* ap_password)
{
    esp_err_t err;
    ESP_LOGE(__func__,"esp_set_string %s", ap_password);
    err = nvs_set_string(stringlify(WAP), stringlify(WAP_PASS), ap_password);
    if (err != ESP_OK) {
         ESP_LOGE(__func__,
                "Error: to settin AP password to the nvs %s",
                esp_err_to_name(err));
    }
    else {
       err = init_ap();
       if (err != ESP_OK) {
            ESP_LOGE(__func__,
                "Error: to the init the wifi %s",
                esp_err_to_name(err));
           return err;
       }
    }
    return ESP_OK;
}

esp_err_t change_wifi_sta_pass (char* sta_password)
{
    esp_err_t err;
    ESP_LOGE(__func__,"password %s", sta_password);
    err = nvs_set_string(stringlify(WSTA), stringlify(WSTA_PASS), sta_password);
    if (err != ESP_OK)
    {
        ESP_LOGE(__func__,
                "Error: to setting STA ssid to the nvs %s",
                esp_err_to_name(err));
    }
    else {
       err = init_sta();
       if (err != ESP_OK) {
           ESP_LOGE(__func__,
                "Error: to the init the wifi %s",
                esp_err_to_name(err));
           return err;
       }
    }
    return ESP_OK;
}

esp_err_t select_wifi_channel (uint8_t channel)
{
    if (channel == 0 || channel > 7 ) {
         ESP_LOGE(__func__,
                "Error: wrong channel, please select a channel in the range of 1 to 7");
        return ESP_FAIL;
    }

    esp_err_t err = nvs_set_uint8(stringlify(WAP), stringlify(WAP_CHAN), channel);
    if (err != ESP_OK)
    {
        ESP_LOGE(__func__,
                "Error: to setting AP Channel to the nvs %s",
                esp_err_to_name(err));
        return err;
    }

    err = set_wifi_mode();
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to the init the wifi %s",
                esp_err_to_name(err));
        return err;
    }

    return ESP_OK;
}

esp_err_t set_ap_max_connection (uint8_t max_connection)
{
    if (max_connection == 0 || max_connection > 4) {
         ESP_LOGE(__func__,
                "the connections must be between 1 and 4");
        return ESP_FAIL;
    }

    esp_err_t err = nvs_set_uint8(stringlify(WAP), stringlify(WAP_MAXCON), &max_connection);
    if (err != ESP_OK) {

        ESP_LOGE(__func__,
                "Error: to setting AP max connections to the nvs %s",
                esp_err_to_name(err));
        return err;
    }
    err = set_wifi_mode();
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to the init the wifi %s",
                esp_err_to_name(err));
        return err;
    }

    return ESP_OK;
}


esp_err_t change_ap_auth (uint8_t auth)
{
    if(auth < 10) {
        ESP_LOGE(__func__, "incorrect auth mode verify the doc and check the auth types");
    }

    esp_err_t err = nvs_set_uint8(stringlify(WAP), stringlify(WAP_AUTH),auth);
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to setting AP auth to the nvs %s",
                esp_err_to_name(err));
    }

    err = set_wifi_mode();
    if (err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to the init the wifi %s",
                esp_err_to_name(err));
        return err;
    }

    return ESP_OK;
}

esp_err_t change_wifi_mode(int mode)
{
    esp_err_t err;
    if(mode != WIFI_MODE_APSTA &&  mode != WIFI_MODE_AP && mode != WIFI_MODE_STA) {
        ESP_LOGE(__func__, "the wifi mode received is incorrect");
        return ESP_FAIL;
    }
    err = nvs_set_uint8(stringlify(WIFI),
                        stringlify(WIFI_MODE),
                        mode);

    if(err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to set wifi mode %s",
                esp_err_to_name(err));
        return ESP_FAIL;
    }
    err = set_wifi_mode();
    if(err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to the init the wifi %s",
                esp_err_to_name(err));
        return ESP_FAIL;
    }

   return ESP_OK;
}

esp_err_t set_wifi_mode ()
{
    int mode = 0;
    esp_err_t err = nvs_get_uint8(stringlify(WIFI), stringlify(WIFI_MODE), &mode);

    if(err != ESP_OK && mode == WIFI_MODE_NULL) {

#if WSTA_ENABLED && WAP_ENABLED
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(init_sta());
    ESP_ERROR_CHECK(init_ap());
    return ESP_OK;

#elif WAP_ENABLED
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(init_ap());
    return ESP_OK;

#elif WSTA_ENABLED
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(init_sta());
    return ESP_OK;

#else
    return ESP_FAIL;

#endif
    }
    else {
        ESP_ERROR_CHECK(esp_wifi_set_mode(mode));
        if (mode == WIFI_MODE_APSTA) {
            ESP_ERROR_CHECK(init_sta());
            ESP_ERROR_CHECK(init_ap());
        }

        if (mode == WIFI_MODE_STA) {
            ESP_ERROR_CHECK(init_sta());
        }

        if (mode == WIFI_MODE_AP) {
            ESP_ERROR_CHECK(init_ap());
        }
    }

    return ESP_OK;
}

esp_err_t wifi_restore_default()
{
   esp_err_t err;
   err = set_default_credentials();
   if(err != ESP_OK ) {
        ESP_LOGE(__func__,
                "Error: to the set init crendentials %s",
                esp_err_to_name(err));
       return err;
   }
   err = set_wifi_mode();
   if(err != ESP_OK ) {
       ESP_LOGE(__func__,
                "Error: to the init the wifi %s",
                esp_err_to_name(err));
       return err;
   }

   return ESP_OK;
}

esp_err_t wifi_init(void)
{
    ESP_LOGI(__func__, "Starting the WiFi module");

    if (nvs_init() != ESP_OK)
    {
        ESP_LOGE(__func__, "Starting the [N]on [V]olarile [S]torage");
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

    if (!is_configured) {
        set_default_credentials();
    }

   esp_err_t err = set_wifi_mode();
    if(err != ESP_OK) {
        ESP_LOGE(__func__,
                "Error: to the init the wifi %s",
                esp_err_to_name(err));
        return err;
    }

   return ESP_OK;
}
