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
 * @brief Function to get all params saved inside the mtd storage.
 * This will read and process all saved data to set a mesh device
 * to the last configuration.
 *
 */
void load_settings(void);

/**
 *
 * @brief  Data type to get a brew description of the sensors
 */
typedef struct {
    uint8_t class_sensor : 1; /*!< class of the sensor, e.g TEMPERATURE (0), MOISTURE (1)*/
    uint8_t sensor_type : 1;  /*!< Soil(0) or Air(1)*/
    uint8_t pin : 6;          /*!< Input pin -> Max pin 63*/
} sensors_t;

/**
 * @brief  struct to save interface data settings.
 */
typedef struct {
    uint8_t id;
    uint8_t type;
    uint16_t channel; /*!< Channel of the radio */
    int16_t tx_power; /*!< The transmit power from a radio interface*/
    uint16_t freq;
} settings_ifaces_t;

#define IF_KEY (uint8_t*)("IFKEY")

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
