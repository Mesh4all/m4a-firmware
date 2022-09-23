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

void net_tools_usage(void) {
    puts("net-tools usage:");
    puts("\t ToDo: list-of-commands");
    puts("\t ToDo: example-of-use");
    puts("");
}

int net_tools_cmd(int argc, char **argv) {
    (void)argc;
    (void)argv;

    if ((argc < 2) || (argc > 2)) {
        puts("net-tools command needs at least one argument");
        net_tools_usage();
        return 1;
    }

    if ((strcmp(argv[1], "help") == 0)) {
        net_tools_usage();
        return 0;
    }
    return 0;
}
