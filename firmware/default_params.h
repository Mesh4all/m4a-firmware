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
 * @ingroup     m4a-firmware
 * @{
 * @file
 * @name    Defaults params for the m4a firmware
 * @author  xkevin190 <kevinvelasco193@gmail.com>
 */

#ifndef DEFAULT_PARAMS_H
#define DEFAULT_PARAMS_H

#include "storage.h"
#ifdef MODULE_NETDEV_DEFAULT
#include "net/netdev/ieee802154.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief  Data type to get a brew description of the sensors
 */
typedef struct {
    uint8_t class_sensor : 1; /*!< class of the sensor, e.g TEMPERATURE (0), MOISTURE (1)*/
    uint8_t sensor_type : 1;  /*!< Soil(0) or Air(1)*/
    uint8_t pin : 6;          /*!< Input pin -> Max pin 63*/
} sensors_t;

/**
 * @brief  struct to save all data of the firmware Mesh4all
 */
typedef struct {
#if ((MODULE_RADIO) || (DOXYGEN))
    int16_t radio_tx_power : 6; /*!< The transmit power from a radio interface*/
    uint16_t subghz : 1;        /*!< Represent if the interface is active in 2.4 sub-GHZ mode*/
    uint16_t channel : 5;       /*!< Channel of the radio */

#if ((MODULE_RPL_PROTOCOL) || (DOXYGEN))
    uint16_t rpl_mode : 1; /*!< If the system has an RPL in mode `DAG` or `DODAG`*/
    uint8_t rpl_instance;  /*!< Number of instance of DODAG*/
    uint16_t pan_id;       /*!< The Personal Area Network where is located the RPL network*/
    int16_t : 1;           /*!< Empty bit (Boolean Expression Could be used)*/
#else
    int16_t : 2;
#endif
#endif
    /* peripherals */
    uint8_t amount_sensors : 5; /*!< Amount of sensors referenced with the number of pins
                                (64 pins)*/
    uint8_t wifi_subsys : 1;    /*!< The Wifi-subsystem is connected*/
    uint8_t uniqueid_mode : 2;  /*!< The unique id mode STATIC(0), RANDOM(1), MANUAL(2)*/
    sensors_t sensors[2];       /*!< Sensor set in the firmware*/
} storage_data_t;

/**
 * @name storage address this address will be the storage keys
 * @{
 */
/**
 * @brief first address of the page this name is optional must be change in the future
 */
#define ADDRESS1 (uint32_t) flashpage_addr(LAST_AVAILABLE_PAGE)

/**
 * @brief second address of the page this name is optional must be change in the future
 */
#define ADDRESS2 (uint32_t) flashpage_addr(LAST_AVAILABLE_PAGE - 1)

/**
 * @brief third address of the page this name is optional must be change in the future
 */
#define ADDRESS3 (uint32_t) flashpage_addr(LAST_AVAILABLE_PAGE - 2)
/** @} */

#ifdef __cplusplus
}
#endif
#endif /* DEFAULT_PARAMS_H */
/** @} */
