/*
 * Copyright (c) 2021 Mesh4all <mesh4all.org>
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
 * @brief       Main firmware file
 *
 * @copyright   Copyright (c) 2021 Mesh for all
 * @author      luisan00 <luisan00@hotmail.com>
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "shell_commands.h"
#include "msg.h"

#include "log.h"

int dummy_cmd(int argc, char **argv)
{

    return 0;
}

static shell_command_t shell_extended_commands[] = {
    {"dummy_cmd", "dummy cmd descrption", dummy_cmd},
    {NULL, NULL, NULL}};

int main(void)
{
    LOG_INFO("~~ Welcome to Mesh4all ~~\n");

    /* Start shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];

    shell_run(shell_extended_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
