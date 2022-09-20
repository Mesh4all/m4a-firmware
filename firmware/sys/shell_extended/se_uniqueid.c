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
#include "uniqueid.h"

void uid_usage(void) {
    puts("Uniqueid Tool");
    puts("Usage: uid [static|rand]");
    puts("");
}

int uid_cmd(int argc, char **argv) {
    (void)argc;
    (void)argv;

    if ((argc < 2) || (argc > 2) || (strcmp(argv[1], "help") == 0)) {
        uid_usage();
        return 0;
    }

    if (strcmp(argv[1], "static") == 0) {
        puts("ToDo: return static id (cpu-based)");
    } else if (strcmp(argv[1], "random") == 0) {
        puts("Todo: return random id");
    }

    return 0;
}
