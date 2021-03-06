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

#include <stdint.h>
#include <stdio.h>
#include "net/gnrc/netif.h"
#include "net/gnrc/rpl.h"
#include "rpl_protocol.h"
#include "radio.h"
#include "net_tools.h"
#include "uniqueid.h"

int8_t rpl_init(kernel_pid_t iface_pid) {
    if (gnrc_netif_get_by_pid(iface_pid) == NULL) {
        puts("unknown interface specified");
        return -1;
    }

    gnrc_rpl_init(iface_pid);
    printf("successfully initialized RPL on interface %d\n", iface_pid);
    return 0;
}

int8_t gnrc_rpl_dodag_root(uint8_t dodag_instance, ipv6_addr_t *root_address) {

    gnrc_rpl_instance_t *inst = gnrc_rpl_root_init(dodag_instance, root_address, false, false);
    if (inst == NULL) {
        char addr_str[IPV6_ADDR_MAX_STR_LEN];
        printf("error: could not add DODAG (%s) to instance (%d)\n",
               ipv6_addr_to_str(addr_str, root_address, sizeof(addr_str)), dodag_instance);
        return -1;
    }

    printf("successfully added a new RPL DODAG\n");
    return 0;
}

int8_t rpl_dodag_remove(uint8_t instance_id) {
    gnrc_rpl_instance_t *inst;

    if ((inst = gnrc_rpl_instance_get(instance_id)) == NULL) {
        printf("error: could not find the instance (%d)\n", instance_id);
        return -1;
    }

    if (gnrc_rpl_instance_remove(inst) == false) {
        printf("error: could not remove instance (%d)\n", instance_id);
        return -1;
    }

    printf("success: removed instance (%d)\n", instance_id);
    return 0;
}

int8_t rpl_setup(uint8_t mode) {
    int8_t err = 0;
    ipv6_addr_t ip;
    if (mode > 1) {
        printf("Error: Rpl mode not accepted\n");
    }
    err = initial_radio_setup();
    if (err != 0) {
        printf("Error: couldn't init the radio module .\n");
        return -1;
    }
    kernel_pid_t iface_index = get_ieee802154_iface();
    if (iface_index == -1) {
        printf("Error: could not get the iface.\n");
        return -1;
    }
    err = rpl_init(iface_index);
    if (err != 0) {
        printf("Error: couldn't init the RPL .\n");
        return -1;
    }
    if (mode == DODAG || CONFIG_IS_DODAG == DODAG) {
        err = get_ipv6_global(iface_index, &ip);
        if (err < 0) {
            subnet_to_ipv6(&ip);
            if (gnrc_netif_ipv6_addr_add(gnrc_netif_get_by_pid(iface_index), &ip, 64,
                                         GNRC_NETIF_IPV6_ADDRS_FLAGS_STATE_VALID) < 0) {
                printf("Error: Couldn't add IPv6 global address\n");
                return -1;
            }
        }
        err = gnrc_rpl_dodag_root(CONFIG_DODAG_INSTANCE, &ip);
        if (err < 0) {
            printf(
                "Error: Rpl root node ipv6 couldn't set ipv6 global address, File: %s line: %d\n",
                __FILE__, __LINE__);
            return -1;
        }
    }
    return 0;
}
