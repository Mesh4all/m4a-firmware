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
#include "net/gnrc/netif.h"
#include "net/netif.h"
#include "net/netdev/ieee802154.h"
#include "net/gnrc.h"
#include "uniqueid.h"
#include "net/netdev.h"
#include "radio.h"

static uint8_t radio_devices[] = {
    NETDEV_AT86RF215,
    NETDEV_AT86RF2XX,
};

int8_t get_ieee802154_iface(void) {
    int max_ifaces = gnrc_netif_numof();
    if (max_ifaces > 0) {
        gnrc_netif_t *iface;
        for (uint8_t i = 0; i < ARRAY_SIZE(radio_devices); i++) {
            iface = gnrc_netif_get_by_type(radio_devices[i], NETDEV_INDEX_ANY);
        }
        if (iface != NULL) {
            return iface->pid;
        } else {
            return -1;
        }
    }
    return -1;
}

int8_t set_global_ipv6_to_radio(void) {
    ipv6_addr_t global_ipv6 = {
        .u8 = {0},
    };
    subnet_to_ipv6(&global_ipv6);

    int ieee802154_iface = get_ieee802154_iface();
    if (ieee802154_iface == -1) {
        printf("Error: could not get the iface.\n");
        return -1;
    }

    gnrc_netif_t *iface = gnrc_netif_get_by_pid(ieee802154_iface);
    if (iface == NULL) {
        printf("Error: interface doesn't exists.\n");
        return -1;
    }

    /* Add node IPv6 global address */
    if (gnrc_netif_ipv6_addr_add(iface, &global_ipv6, 64, GNRC_NETIF_IPV6_ADDRS_FLAGS_STATE_VALID) <
        0) {
        printf("Error: Couldn't add IPv6 global address\n");
        return -1;
    }

    return 0;
}

int8_t get_netopt_tx_power(int16_t txpower) {
    int res;
    int index = get_ieee802154_iface();
    netif_t *iface = netif_get_by_id(index);

    res = netif_get_opt(iface, NETOPT_TX_POWER, 0, &txpower, sizeof(txpower));
    if (res >= 0) {
        printf(" TX-Power: %" PRIi16 "dBm \n", txpower);
    }else {
        return -1;
    }

    return 0;
}

int8_t get_netopt_channel(int16_t channel) {
    int res;
    int index = get_ieee802154_iface();
    netif_t *iface = netif_get_by_id(index);

    res = netif_get_opt(iface, NETOPT_CHANNEL, 0, &channel, sizeof(channel));
    if (res >= 0) {
        printf(" Channel: %" PRIi16 " \n", channel);
    }else {
        return -1;
    }

    return 0;
}

int8_t set_netopt_tx_power(int16_t txpower) {
    int res;
    int index = get_ieee802154_iface();
    netif_t *iface = netif_get_by_id(index);
    if (txpower < MIN_TX_POWER || txpower > MAX_TX_POWER) {
        printf("Error: the txPower must be between %d dbm and %d dbm \n",
        MIN_TX_POWER, MAX_TX_POWER);
        return -1;
    }

    res = netif_set_opt(iface, NETOPT_TX_POWER, 0, &txpower, sizeof(txpower));
    if (res >= 0) {
        printf(" TX-Power: %" PRIi16 "dBm \n", txpower);
    } else {
        return -1;
    }

    return 0;
}

int8_t set_netopt_channel(int16_t channel) {
    int res;
    int index = get_ieee802154_iface();
    netif_t *iface = netif_get_by_id(index);
    if (channel < 11 || channel > 26) {
        printf("Error: the channels must be between 11 and  26 \n");
        return -1;
    }
    res = netif_set_opt(iface, NETOPT_CHANNEL, 0, &channel, sizeof(channel));
    if (res >= 0) {
        printf(" channel: %" PRIi16 " \n", channel);
    } else {
        return -1;
    }

    return 0;
}

int8_t set_sleep_interface(void) {
    int ieee802154_iface = get_ieee802154_iface();
    netopt_state_t state = NETOPT_STATE_SLEEP;
    if (ieee802154_iface == -1) {
        printf("Error: could not get the iface.\n");
        return -1;
    }

    netif_t *netif = netif_get_by_id(ieee802154_iface);
    int err = netif_set_opt(netif, NETOPT_STATE, 0, &state, sizeof(netopt_state_t));
    if (err < 0) {
        printf("Error: failed to the change status interface.\n");
        return -1;
    }
    return 0;
}

int8_t set_awake_interface(void) {
    int ieee802154_iface = get_ieee802154_iface();
    netopt_state_t state = NETOPT_STATE_IDLE;
    if (ieee802154_iface == -1) {
        printf("Error: could not get the iface.\n");
        return -1;
    }

    netif_t *netif = netif_get_by_id(ieee802154_iface);
    int err = netif_set_opt(netif, NETOPT_STATE, 0, &state, sizeof(netopt_state_t));

    if (err < 0) {
        printf("Error: failed to the change status interface.\n");
        return -1;
    }
    return 0;
}

int8_t initial_radio_setup(void) {
    int err;
    int16_t radio_tx = CONFIG_TX_POWER;
    int16_t radio_channel = CONFIG_RADIO_CHANNEL;

    err = set_global_ipv6_to_radio();
    if (err == -1) {
        printf("Error: Failed to add global address.\n");
        return err;
    }

    err = set_netopt_tx_power(radio_tx);
    if (err == -1) {
        printf("Error: Failed to add TX_power.\n");
        return err;
    }

    err = set_netopt_channel(radio_channel);
    if (err == -1) {
        printf("Error: Failed to add CHANNEL.\n");
        return err;
    }

    return 0;
}
