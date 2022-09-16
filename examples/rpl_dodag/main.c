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
 * @brief       Example RPL DODAG
 *
 * @author      xkevin190 <kevinvelasco193@gmail.com>
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "log.h"
#include "rpl_protocol.h"
#include "net_tools.h"
#include "radio.h"

#define MAIN_QUEUE_SIZE (8)
#ifndef CONFIG_DODAG_IPV6_ADDRESS
#define CONFIG_DODAG_IPV6_ADDRESS "2001:db8::1"
#endif

msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

int main(void) {
    LOG_INFO("~~ Welcome to rpl exam ~~\n");
    /* Start shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    ipv6_addr_t addr;
    ipv6_addr_from_str(&addr, CONFIG_DODAG_IPV6_ADDRESS);
    int8_t iface_idx = get_ieee802154_iface();
    set_ipv6_global(iface_idx, addr, 64);
    rpl_setup(DODAG);
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
