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
 * @ingroup     examples_shell_extended
 *
 * @brief       shows some uses of **shell_tools** module
 * @author      luisan00    <luisan00@hotmail.com>
 */

#include <stdio.h>
#include "shell.h"
#include "msg.h"
#include "shell_extended.h"

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

int main(void) {
    puts("Shell extended example\n");

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    /* start the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_extended_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
