/*
 * Copyright (C) 2022 Mesh4all.org <mesh4all.org>
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "msg.h"
#include "shell.h"
#include "border_router.h"
#include "radio.h"
#include "rpl_protocol.h"
#include "chamos.h"

#define MAIN_QUEUE_SIZE (8)

msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
shell_command_t shell_extended_commands[] = {{NULL, NULL, NULL}};
int packet_dump_init(void);

int setup(void) {
#if (CONFIG_IS_DODAG)
    ipv6_addr_t addr = {.u8 = {0}};
    ipv6_addr_from_str(&addr, CONFIG_ADDRESS_IPV6_WIRELESS);
    border_router_setup(addr, 64, CONFIG_WIRELESS_INTERFACE);
#endif
    rpl_setup(CONFIG_IS_DODAG);
    return 0;
}

int main(void) {
    /* Start shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];

    puts("Generated Mesh4all application: 'border_router'");
    /* Select the iface for chamo-server */
    gnrc_netif_t *iface = gnrc_netif_get_by_type(NETDEV_ANY, NETDEV_INDEX_ANY);
    /* Init chamo-server */
    chamos_init(6977, iface);
    /* Execute setup routines */
    setup();

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    init_br_routing();
    shell_run(shell_extended_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}