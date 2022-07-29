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
 * @{
 * @ingroup     radio_module
 * @file        radio_params.h
 * @brief       this module content all params to set the radio
 * @author      xkevin190 <kevinvelasco193@gmail.com.com>
 * @author      eduazocar <eduazocarv@gmail.com>
 * @author      RocioRojas <rociorojas391@gmail.com>
 *
 */

#ifndef RADIO_PARAMS_H
#define RADIO_PARAMS_H

#include "net/gnrc.h"
#include "net/netdev.h"
#include "net/gnrc/netif.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MODULE_AT86RF233
#define TX_POWER_MAX (4)
#define TX_POWER_MIN (-17)
#endif

#ifdef MODULE_AT86RF215
#define TX_POWER_MAX (3)
#define TX_POWER_MIN (-28)
#endif

#ifdef MODULE_CC2538
#define TX_POWER_MAX (7)
#define TX_POWER_MIN (-24)
#endif

/** @name  Driver_Default_Params Tx_power_limits
 *  @note  Every param is empty when the device is unknown
 * @{
 */

#ifndef TX_POWER_MAX
#define TX_POWER_MAX (0) /*!< Max Tx_power value from the driver */
#endif

#ifndef TX_POWER_MIN
#define TX_POWER_MIN (0) /*!< Min Tx_power value from the driver */
#endif

/**@}*/

/** @name  Driver_Default_Params Tx_power
 *  @note  Every param is empty when the device is unknown
 * @{
 */
#ifndef CONFIG_TX_POWER
#define CONFIG_TX_POWER (0)     /*!< Default system tx power \
                                when the system starts running */
#endif

#ifndef CONFIG_RADIO_CHANNEL
#define CONFIG_RADIO_CHANNEL (26)   /*!< Default start radio channel, \
                                      This refers to the 2.4 GHz band */
#endif

#ifndef CONFIG_SUB_RADIO_CHANNEL
#define CONFIG_SUB_RADIO_CHANNEL (0) /*!< Default start radio channel, \
                                     this refers to under the 2.4 GHz band*/
#endif
/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* RADIO_PARAMS_H */
/**@}*/
