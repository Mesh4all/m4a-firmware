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

/**
 * @brief Simple solution to use custom settings "almost" safely.
 */
#define USE_DEV_CONFIG 0

/**
 * @brief If it's #1 (true) then **dev-config.h** will be used.
 */
#if (USE_DEV_CONFIG == 1)
#include "dev-config.h"
#else

/**
 * @name Factory values for the access point mode
 * @{
 */
#define WAP                             /*!< NVS namespace for AP settings*/
#define WAP_CHAN 6                      /*!< Default AP channel */
#define WAP_SSID "m4a-ap"               /*!< Default AP SSID */
#define WAP_PASS "change-me"            /*!< Default AP password*/
#define WAP_AUTH WIFI_AUTH_WPA_WPA2_PSK /*!< Default AP Authentication mode */
#define WAP_MAXCON 4                    /*!< Max.connections for the AP */
#define WAP_ENABLED 1                   /*!< Access Point mode is enabled */
/** @} */

/**
 * @name Factory values for the station mode
 * @{
 */
#define WSTA                            /*!< NVS namespace for WiFi station mode settings */
#define WSTA_SSID "m4a-ap"              /*!< Default SSID for the station mode */
#define WSTA_PASS "change-me"           /*!< Default password for station */
#define WSTA_RETRIES 5                  /*!< Max. num of retries */
#define WSTA_ENABLED 1                  /*!< Station mode is enabled */
/** @} */

/**
 * @name Factory values for other wifi settings 
 * @{
 */
#define WIFI
#define WIFI_MODE
/** @} */

/**
 * @name Factory values for NVS settings
 * @{
 */
#define NVS
#define NVS_IS_INIT 0
/** @} */
#endif

/**
 * @name literal to string and string to literal macro helpers
 * @{
 */
#define literal_to_str(literal) #literal                /*!< Transform literal to string */
#define nvs_key_to_name(...) literal_to_str(__VA_ARGS_) /*!< A nonsense for the nvs component */
#define at_cmd_to_name(...) literal_to_str(__VA_ARGS__) /*!< Another one for the uart(at) component*/
    /** @} */

#ifdef __cplusplus
}
#endif
#endif /* DEFAULT_PARAMS_H */
/** @} */
