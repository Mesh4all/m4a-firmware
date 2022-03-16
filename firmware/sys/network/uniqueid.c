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
 * @brief  cpuid address
 *
 * @author  RocioRojas <rociorojas391@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uniqueid.h"

void cpuid_to_ipv6(ipv6_addr_t *addr) {

    char addr_cpu[CPUID_LEN] = {0};
    CPUID(addr_cpu);
    ipv6_addr_t header = {
        .u8 = {0},
    };
    ipv6_addr_from_str(&header, CONFIG_HEADER_ADDRESS_ID);
    memcpy((char *)addr->u8, (char *)header.u8, 4);
    strncat((char *)addr->u8, addr_cpu, 4);
}
