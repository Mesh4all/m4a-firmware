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
 * @brief   defaults params for wifi
 * @ingroup     wifisubsys
 * @{
 * @author  xkevin190 <kevinvelasco193@gmail.com>
 */

#ifndef DEFAULT_PARAMS_H
#define DEFAULT_PARAMS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define WIFI_ENABLE_STA 1                 /*!< Is enable the sta wifi? */
#define WIFI_ENABLE_AP 1                   /*!< Is enable the AP wifi? */
#define AP_CHANNEL 6                       /*!< AP Channel */
#define AP_MAXCONN 4                       /*!< AP Maximum connections */
#define AP_AUTHMODE WIFI_AUTH_WPA_WPA2_PSK /*!< AP Authentication mode */
#define STA_WIFI_SSID "m4a-wifi"           /*!< default STA ssid */
#define STA_WIFI_PASSWORD "m4a-default"    /*!< default STA password */
#define ESP_MAXIMUM_RETRY 5
#define AP_WIFI_SSID "m4a_AP"           /*!< default AP ssid */
#define AP_WIFI_PASSWORD "m4a-default"    /*!< default AP password */

#define NVS_APP_NAMESPACE "m4a-firmware"           /*!< NVS application namespace */
#define NVS_IS_CONFIGURED_KEY "is_configured"       /*!< Is wifi configured? */
#define USER_STA_NAME_KEY "username_sta"      /*!< user_name sta key for nvs */
#define USER_STA_PASSWORD_KEY "password_sta"  /*!< user_password sta key for nvs */
#define USER_AP_PASSWORD_KEY "password_ap"    /*!< user_name ap key for nvs */
#define USER_AP_NAME_KEY "username_ap"        /*!< user_password ap key for nvs */
#define MAX_USER_NAME_LENGTH 32       /*!< length of thr username */
#define MAX_USER_PASSWORD_LENGTH 64   /*!< max length of password for user */

#ifdef __cplusplus
}
#endif
#endif /* DEFAULT_PARAMS_H */
/** @} */
