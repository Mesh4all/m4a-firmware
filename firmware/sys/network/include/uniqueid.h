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
 * @ingroup     network
 * @{
 * @file
 * @brief       cpuid address
 *
 * @author      RocioRojas <rociorojas391@gmail.com>
 *
 */
#ifndef UNIQUEID_H
#define UNIQUEID_H

#include "periph/cpuid.h"
#include "board.h"
#include "net/ipv6/addr.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function get ipv6 address
 * @param [out] addr Address in ipv6 format
 */
void cpuid_to_ipv6(ipv6_addr_t *addr);

#ifdef __cplusplus
}
#endif
#endif /* UNIQUEID_H */
/** @} */
