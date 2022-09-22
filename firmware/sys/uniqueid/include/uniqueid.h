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
 * @ingroup     uniqueid
 * @{
 * @file
 * @brief       IPV6 address
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

#define LAST_OCTECTS (8)
#define OCTETS_BYTE_SIZE (8)

#ifdef CONFIG_MODE_STATIC
#define KCONFIG_UID_OPTION (UNIQUEID_STATIC_MODE)
#endif
#ifdef CONFIG_MODE_RANDOM
#define KCONFIG_UID_OPTION (UNIQUEID_RANDOM_MODE)
#endif

/**
 * @name Uniqueid modes
 * @{
 */
typedef enum {
    UNIQUEID_STATIC_MODE = 0,
    UNIQUEID_RANDOM_MODE,
} uniqueid_mode_t;

/**@}*/

/**
 * @brief  Get the CPU unique ID of this platform
 * This function gets base cpu address using cpuid_get
 *
 * @param  id  pointer to char array with CPUID_LEN size.
 *
 */
#define CPUID(id) cpuid_get(id)

/**
 * @brief  union to get 32 bit random number and convert to a byte array.
 */

union random_buff {
    uint8_t u8[8];   /*!< unint8 buff*/
    uint32_t u32[2]; /*!< uint32 address*/
};
/**
 * @brief This function get ipv6 address (mode: static (default), random, manual)
 * @param [out] addr Address in ipv6 format
 */
void get_uid_ipv6(ipv6_addr_t *addr, uniqueid_mode_t mode);

uint32_t get_uid_seed(void *val, uint8_t len, uint8_t uid_mode);
#ifdef __cplusplus
}
#endif
#endif /* UNIQUEID_H */
/** @} */
