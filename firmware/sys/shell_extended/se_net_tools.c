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
 * @brief   Extends the shell functionality for uniqueid
 * @author  Luis A. Ruiz    <luisan00@hotmail.com>
 * @author  Eduardo Azócar  <eduazocarv@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include "net_tools.h"
#if MODULE_UNIQUEID
#include "uniqueid.h"
#endif

#define ENABLE_DEBUG 0
#include "debug.h"

void show_iface_info(kernel_pid_t iface_idx);
void check_inp_addr(char *addr, ipv6_addr_t *ip, uint8_t *pfx);

void net_tools_usage(void) {
    puts("net-tools usage:");
    puts("\tlist-of-commands");
    puts("\t if [iface id] [add]");
    puts("\t\t\t -[ipv6 unicast addr/network prefix length]");
#if MODULE_UNIQUEID
    puts("\t\t\t -[ipuid] [header/prefix] [static|random]");
#endif
    puts("");
}

int net_tools_cmd(int argc, char **argv) {
    (void)argc;
    (void)argv;

    if ((strcmp(argv[1], "help") == 0)) {
        net_tools_usage();
        return 0;
    }
    if ((strcmp(argv[2], "add") == 0)) {
        int if_idx = atoi(argv[1]);
        char ip[40] = "";
        ipv6_addr_t addr;
        uint8_t prefix;
        if ((netif_get_by_id(if_idx)) == NULL) {
            puts("Interface doesn't exist");
            return -1;
        }
#if MODULE_UNIQUEID
        if (strcmp(argv[3], "ipuid") == 0) {
            check_inp_addr(argv[4], &addr, &prefix);
            if ((strcmp(argv[5], "static") == 0)) {
                set_ipv6_by_uid(if_idx, &addr, prefix, UNIQUEID_STATIC_MODE);
                DEBUG("ipv6 address: %s\n", ipv6_addr_to_str(ip, &addr, sizeof(ip)));
                return 0;
            }
            if ((strcmp(argv[5], "random") == 0)) {
                set_ipv6_by_uid(if_idx, &addr, prefix, UNIQUEID_RANDOM_MODE);
                DEBUG("ipv6 address: %s\n", ipv6_addr_to_str(ip, &addr, sizeof(ip)));
                return 0;
            } else {
                puts("Ipuid (Ip by Unique ID) mode wasn't selected");
                return -1;
            }
        }
#endif
        check_inp_addr(argv[3], &addr, &prefix);
        set_ipv6_global(if_idx, addr, prefix);
        DEBUG("ipv6 address: %s\n", ipv6_addr_to_str(ip, &addr, sizeof(ip)));
        return 0;
    }
    if (strcmp(argv[2], "show") == 0) {
        uint8_t if_idx = atoi(argv[1]);
        if ((netif_get_by_id(if_idx)) == NULL) {
            puts("Interface doesn't exist");
            return -1;
        }
        show_iface_info(if_idx);
        return 0;
    }

    if ((argc < 5) || (argc > 5)) {
        puts("net-tools command needs at least one argument");
        net_tools_usage();
        return 1;
    }
    return 0;
}

void show_iface_info(kernel_pid_t iface_idx) {
    /*ToDo show of an interface its specifications -> Physical info*/
    printf("ToDo: Show an interface\n");
    if (iface_idx == 0) {
        /*ToDo show all interfaces and their physical features.*/
    }
}

void check_inp_addr(char *addr, ipv6_addr_t *ip, uint8_t *pfx) {
    uint8_t strlen = 0;
    char *prefix;
    while (*(addr + strlen) != '/') {
        strlen++;
        if (*(addr + strlen) == '\0') {
            printf("Wrong input address. Address must be e.g: 2001:db8::/64\n");
            return;
        }
    }
    strlen++;
    prefix = addr + strlen;
    *(addr + strlen - 1) = '\0';
    ipv6_addr_from_str(ip, addr);
    *pfx = atoi(prefix);
}
