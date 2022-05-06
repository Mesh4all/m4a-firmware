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
 * @brief   CHAMOC_main
 *
 * @author  eduazocar<eduazocarv@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chamoc_msg.h"
#include "nib.h"

int main(int argc, char **argv) {
    if (argc < 2 || argc > 6) {
        printf("Usage <nib> [add, del] [iface] [ipv6_address] [prefix]\n");
    }
    else if (strcmp(argv[1], "nib") == 0) {
        if (argc < 6) {
            printf("Wrong Passed\n");
        } else if (strcmp(argv[2], "add") == 0) {
            nib_req(argv[3], argv[4], (uint8_t)atoi(argv[5]), MSG_NIB_ADD);
        } else if (strcmp(argv[2], "del") == 0) {
            nib_req(argv[3], argv[4], (uint8_t)atoi(argv[5]), MSG_NIB_DEL);
        }
    }

    return 1;
}
