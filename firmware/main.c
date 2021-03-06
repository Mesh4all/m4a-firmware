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
 * @brief       Main firmware file
 *
 * @author      luisan00 <luisan00@hotmail.com>
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "msg.h"

#include "log.h"
#include "board.h"

int main(void)
{
    LOG_INFO("~~ Welcome to Mesh4all ~~\n");
    /* Start shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];

    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
