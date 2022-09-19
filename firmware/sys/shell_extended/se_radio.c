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
 * @brief   Extends functionality on the shell for uniqueid
 * @author  luisan00    <luisan00@hotmail.com>
 */

#include <stdio.h>
#include "radio.h"

int radio_usage(int argc, char **argv) {
    (void) argc;
    (void) argv;
    puts("Low level routines for radio module");
    puts("usage: uniqueid <command> [arguments]");
    puts("commands:");
    puts("\tiface_identify");
    puts("\tget_iface");
    puts("\tget_channel");
    puts("\tget_power");
    puts("\tset_channel [channel_number]");
    puts("\tset_power   [power_value]");
    puts("");
    return 0;
}
