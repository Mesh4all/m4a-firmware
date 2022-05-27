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

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Mesh4all Border Router Example
 *
 * @author      eduazocar <eduardoazocar7@gmail.com>
 *
 * @}
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "shell_commands.h"
#include "msg.h"

#include "log.h"
#include "board.h"

#include "rpl_protocol.h"
#include "border_router.h"
static shell_command_t shell_extended_commands[] = {{NULL, NULL, NULL}};

#ifndef CONFIG_RTR_IPV6
#define CONFIG_RTR_IPV6 "2001:db8:1::1"
#endif

ipv6_addr_t router_addr = {.u8 = {0}};

int main(void) {
    puts("Generated RIOT application: 'border_router_example'");
    ipv6_addr_from_str(&router_addr, CONFIG_RTR_IPV6);
    border_router_add_ipv6(_UNICAST, &router_addr);
#ifdef CONFIG_IS_DODAG
    rpl_setup();
#endif
    char line_buf[SHELL_DEFAULT_BUFSIZE];

    shell_run(shell_extended_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
