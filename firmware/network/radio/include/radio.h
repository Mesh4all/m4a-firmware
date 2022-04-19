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
 * @ingroup     network
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

#ifndef MAX_TX_POWER
#define MAX_TX_POWER (20)
#endif

#ifndef MIN_TX_POWER
#define MIN_TX_POWER (-17)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* These Params should be Set in IEEE802154.H
 *
 * Take a list to every specific radio driver to check
 * hardware limitations to change TX_POWER
 *
 */

/**
 * @brief This function it's to get index of an IEEE802.15.4 interface
 *
 * @return int
 */
int8_t get_ieee802154_iface(void);

/**
 * @brief Set the global ipv6 to radio object
 *
 * @return int
 */
int8_t set_global_ipv6_to_radio(void);

/**
 * @brief this function is used to get data of tx power
 * @param[in] txpower channel you want to get
 */
int8_t get_netopt_tx_power(int16_t txpower);

/**
 * @brief this function is used to get data of channel
 * @param[in] channel channel you want to get
 */
int8_t get_netopt_channel(int16_t channel);

/**
 * @brief this function is used to set the channel
 *
 * @param[in] channel channel you want to set
 * @note the device has channels between 11 to 26
 */
int8_t set_netopt_channel(int16_t channel);

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

#ifdef __cplusplus
}
#endif
#endif /* RADIO_H */
/**@}*/
