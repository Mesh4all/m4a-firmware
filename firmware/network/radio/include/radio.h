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
 * @file        radio.h
 * @brief       this module content all functions of radio
 * @author      xkevin190 <kevinvelasco193@gmail.com.com>
 * @author      eduazocar <eduazocarv@gmail.com>
 * @author      RocioRojas <rociorojas391@gmail.com>
 *
 */

#ifndef RADIO_H
#define RADIO_H

#include "net/gnrc.h"
#include "net/netdev.h"
#include "radio_params.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function it's to get index of an IEEE802.15.4 interface
 *
 * @return int
 */
int8_t get_ieee802154_iface(void);

/**
 * @brief this function is used to get data of tx power
 * @param[in] txpower channel you want to get
 */
int8_t get_netopt_tx_power(int16_t *txpower);

/**
 * @brief this function is used to get data of channel
 * @param[in] channel channel you want to get
 */
int8_t get_netopt_channel(uint16_t *channel);

/**
 * @brief this function is used to set the channel
 *
 * @param[in] channel channel you want to set
 * @note the device has channels between 11 to 26
 */
int8_t set_netopt_channel(uint16_t channel);

/**
 * @brief this function is used to set the tx power
 *
 * @param[in] txpower Tx power you want to set
 * @note the device has power from -17 dbm to 4
 */
int8_t set_netopt_tx_power(int16_t txpower);

/**
 * @brief initiates the necessary functions for the correct operation of the radio.
 * @return int
 */
int8_t initial_radio_setup(void);

/**
 * @brief function used to change the status of the interface to sleep
 * @return int
 */
int8_t set_sleep_interface(void);

/**
 * @brief function used to change the status of the interface to awake
 * @return int
 */
int8_t set_awake_interface(void);

/**
 * @brief function used to determinate if there is a radio multiband interface
 * @return bool (True is Multiband, False is Single-Band)
 */
bool identify_multiple_radio_interface(void);

#ifdef __cplusplus
}
#endif
#endif /* RADIO_H */
       /**@}*/
