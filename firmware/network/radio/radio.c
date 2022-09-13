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
 * @brief  radio
 *
 * @author  xkevin190 <kevinvelasco193@gmail.com>
 * @author  RocioRojas <rociorojas391@gmail.com>
 * @author  eduazocar <eduardo@turpialdev.com>
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "net/gnrc/netif.h"
#include "net/netif.h"
#include "net/netdev/ieee802154.h"
#include "net/gnrc.h"
#include "net/netdev.h"
#include "radio.h"

#if (CONFIG_DEBUG_RADIO) || (DOXYGEN)
/**
 * @brief KCONFIG_PARAMETER TO SET DEBUG MODE
 *
 */
#define ENABLE_DEBUG CONFIG_DEBUG_RADIO
#else
#define ENABLE_DEBUG 0
#endif

#include "debug.h"

static uint8_t radio_devices[] = {NETDEV_AT86RF215, NETDEV_AT86RF2XX, NETDEV_CC2538};

#ifdef CONFIG_MODE_SUB_24GHZ
int8_t subtract_to_interface = 1;
#else
int8_t subtract_to_interface = 0;
#endif

bool identify_multiple_radio_interface(void) {
    int8_t index = get_ieee802154_iface();
    gnrc_netif_t *iface_mult = gnrc_netif_get_by_pid(index + 1);
    if (index != -1) {
        if (iface_mult != NULL) {
            return true;
        }
    }
    return false;
}

int8_t get_ieee802154_iface(void) {
    int max_ifaces = gnrc_netif_numof();
    if (max_ifaces > 0) {
        gnrc_netif_t *iface = NULL;
        for (uint8_t i = 0; i < ARRAY_SIZE(radio_devices); i++) {
            iface = gnrc_netif_get_by_type(radio_devices[i], NETDEV_INDEX_ANY);
            if (iface != NULL) {
                break;
            }
        }
        if (iface != NULL) {
            return iface->pid - subtract_to_interface;
        } else {
            return -1;
        }
    }
    DEBUG("Error: Radio interface doesn't exist, File:%s line:%d function: %s\n", __FILE__,
          __LINE__, __func__);
    return -1;
}

int8_t get_netopt_tx_power(int16_t txpower) {
    int res;
    int index = get_ieee802154_iface();
    if (index < 0) {
        return -1;
    }
    netif_t *iface = netif_get_by_id(index);

    res = netif_get_opt(iface, NETOPT_TX_POWER, 0, &txpower, sizeof(txpower));
    if (res >= 0) {
        DEBUG("TX-Power: %" PRIi16 "dBm \n", txpower);
    } else {
        return -1;
    }

    return 0;
}

int8_t get_netopt_channel(int16_t channel) {
    int res;
    int index = get_ieee802154_iface();
    if (index < 0) {
        return -1;
    }
    netif_t *iface = netif_get_by_id(index);

    res = netif_get_opt(iface, NETOPT_CHANNEL, 0, &channel, sizeof(channel));
    if (res >= 0) {
        DEBUG("Channel: %" PRIi16 " \n", channel);
    } else {
        return -1;
    }

    return 0;
}

int8_t set_netopt_tx_power(int16_t txpower) {
    int res;
    int index = get_ieee802154_iface();
    if (index < 0) {
        return -1;
    }
    netif_t *iface = netif_get_by_id(index);
    if (txpower < TX_POWER_MIN || txpower > TX_POWER_MAX) {
        DEBUG("Error: the txPower must be between %d dbm and %d dbm \n", TX_POWER_MIN,
              TX_POWER_MAX);
        return -1;
    }

    res = netif_set_opt(iface, NETOPT_TX_POWER, 0, &txpower, sizeof(txpower));
    if (res >= 0) {
        DEBUG(" TX-Power: %" PRIi16 "dBm \n", txpower);
    } else {
        return -1;
    }

    return 0;
}

int8_t set_netopt_channel(int16_t channel) {
    int res;
    int index = get_ieee802154_iface();
    if (index < 0) {
        return -1;
    }
    netif_t *iface = netif_get_by_id(index);
#ifdef CONFIG_MODE_SUB_24GHZ
    if (channel < 0 || channel > 10) {
        DEBUG("Error: the channels must be between 0 and  10 \n");
        return -1;
    }
#else
    if (channel < 11 || channel > 26) {
        DEBUG("Error: the channels must be between 11 and  26 \n");
        return -1;
    }
#endif
    res = netif_set_opt(iface, NETOPT_CHANNEL, 0, &channel, sizeof(channel));
    if (res >= 0) {
        DEBUG(" channel: %" PRIi16 " \n", channel);
    } else {
        return -1;
    }

    return 0;
}

int8_t set_sleep_interface(void) {
    int ieee802154_iface = get_ieee802154_iface();
    netopt_state_t state = NETOPT_STATE_SLEEP;
    if (ieee802154_iface == -1) {
        DEBUG("Error: could not get the iface.\n");
        return -1;
    }

    netif_t *netif = netif_get_by_id(ieee802154_iface);
    int err = netif_set_opt(netif, NETOPT_STATE, 0, &state, sizeof(netopt_state_t));
    if (err < 0) {
        DEBUG("Error: failed to the change status interface.\n");
        return -1;
    }
    return 0;
}

int8_t set_awake_interface(void) {
    int ieee802154_iface = get_ieee802154_iface();
    netopt_state_t state = NETOPT_STATE_IDLE;
    if (ieee802154_iface == -1) {
        DEBUG("Error: could not get the iface.\n");
        return -1;
    }

    netif_t *netif = netif_get_by_id(ieee802154_iface);
    int err = netif_set_opt(netif, NETOPT_STATE, 0, &state, sizeof(netopt_state_t));

    if (err < 0) {
        DEBUG("Error: failed to the change status interface.\n");
        return -1;
    }
    return 0;
}

int8_t initial_radio_setup(void) {

    bool is_multy_band = identify_multiple_radio_interface();
    if (is_multy_band) {
#ifdef CONFIG_MODE_SUB_24GHZ
        subtract_to_interface = 1;
#else
        subtract_to_interface = 0;
#endif
    }
    int err;
    int16_t radio_tx = CONFIG_TX_POWER;
    int16_t radio_channel = CONFIG_RADIO_CHANNEL;

    err = set_netopt_tx_power(radio_tx);
    if (err == -1) {
        DEBUG("Error: Failed to add TX_power.\n");
        return err;
    }

    err = set_netopt_channel(radio_channel);
    if (err == -1) {
        DEBUG("Error: Failed to add CHANNEL.\n");
        return err;
    }

    return 0;
}
