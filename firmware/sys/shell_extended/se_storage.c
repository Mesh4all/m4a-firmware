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
 * @brief   Extends the shell functionality for storage
 * @author  Luis A. Ruiz    <luisan00@hotmail.com>
 * @author  Eduardo Azócar  <eduazocarv@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include "storage.h"
#include "storage_register.h"
#include "default_params.h"
#include "radio.h"
#if MODULE_RPL_PROTOCOL
#include "rpl_protocol.h"
#endif
int8_t stg_save_if_settings(void);
int8_t stg_del_if_settings(void);
int8_t stg_load_if_settings(void);
void build_if_settings(settings_ifaces_t *if_sets, uint8_t num_ifaces);

void storage_usage(void) {
    puts("Storage tool");
    puts("Usage:  stg [save|load|show]");
    puts("\t\t\t->[iface]");
    printf("%" PRIu16 " Bytes. \n", sizeof(settings_ifaces_t));
    puts("");
}

int storage_cmd(int argc, char **argv) {
    (void)argc;
    (void)argv;
    if ((argc < 2) || (argc > 2) || (strcmp(argv[1], "help") == 0)) {
        storage_usage();
        return 0;
    }
    if (strcmp(argv[1], "save") == 0) {
        stg_save_if_settings();
    } else if (strcmp(argv[1], "load") == 0) {
        stg_load_if_settings();
    } else if (strcmp(argv[1], "show") == 0) {
        puts("Todo:  show all firmware params saved in mtd storage");
    } else if (strcmp(argv[1], "del") == 0) {
        stg_del_if_settings();
    }
    return 0;
}

int8_t stg_save_if_settings(void) {
    int8_t ret = 0;
    uint8_t ifaces = gnrc_netif_numof();
    settings_ifaces_t settings_if[ifaces];
    build_if_settings(settings_if, ifaces);
    mtd_reg_del(IF_KEY, sizeof(settings_if));
    ret = mtd_save_reg(settings_if, IF_KEY, sizeof(settings_if));
    if (ret < 0) {
        printf("Failed saving/overwriting the register\n");
        return ret;
    }
    return ret;
}

int8_t stg_del_if_settings(void) {
    int8_t ret = 0;
    uint8_t ifaces = gnrc_netif_numof();
    ret = mtd_reg_del(IF_KEY, ifaces * sizeof(settings_ifaces_t));
    return ret;
}

int8_t stg_load_if_settings(void) {
    int8_t ret = 0;
    uint8_t ifaces = gnrc_netif_numof();
    settings_ifaces_t settings_if[ifaces];
    ret = mtd_load_reg(settings_if, IF_KEY, sizeof(settings_if));
    if (ret < 0) {
        printf("The register doesn't exists, first you need to save the interface info\n");
        return ret;
    }
    for (size_t i = 0; i < ifaces; i++) {
        printf("Netif_id: %" PRId16 "\n", settings_if[i].id);
        if (settings_if[i].type == 1) {
            if (set_netopt_tx_power(settings_if[i].tx_power) < 0) {
                printf("Failed: loading radio settings [tx_power]\n");
            }
            if (set_netopt_channel(settings_if[i].channel) < 0) {
                printf("Failed: loading radio settings [channel]\n");
            }
            printf("Type: Radio iface\n");
            printf("id: %" PRId16 "\n", settings_if[i].id);
            printf("Tx power %" PRId16 "\n", settings_if[i].tx_power);
            printf("Channel: %" PRIu16 "\n", settings_if[i].channel);
        }
    }
    return ret;
}

void build_if_settings(settings_ifaces_t *if_sets, uint8_t num_ifaces) {
    gnrc_netif_t *netif = NULL;
    for (uint8_t i = 0; i < num_ifaces; i++) {
        netif = gnrc_netif_iter(netif);
        if (netif->ops != NULL) {
            if_sets[i].id = netif->pid;
        }
        switch (netif->device_type) {
        case NETDEV_TYPE_IEEE802154:
            if_sets[i].type = 1;
            if (get_netopt_tx_power(&if_sets[i].tx_power) < 0) {
                puts("Err Loading Tx_power");
            }
            if (get_netopt_channel(&if_sets[i].channel) < 0) {
                puts("Err Loading ");
            }
            break;
        default:
            if_sets[i].type = 0;
            break;
        }
    }
}
