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
 * @brief This function gets ipv6 address (mode: static (default), random, manual)
 *
 * @author  RocioRojas <rociorojas391@gmail.com>
 * @author  eduazocar <eduazocarv@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uniqueid.h"
#include "random.h"
#include "net/netdev/ieee802154.h"
#include "net/gnrc.h"
#if MODULE_AT86RF2XX || MODULE_AT86RF215
#include "radio.h"
#elif MODULE_PERIPH_HWRNG
#include "periph/hwrng.h"
#endif

#if (CONFIG_DEBUG_UNIQUEID) || (DOXYGEN)
/**
 * @brief KCONFIG_PARAMETER TO SET DEBUG MODE
 *
 */
#define ENABLE_DEBUG CONFIG_DEBUG_UNIQUEID
#else
#define ENABLE_DEBUG 0
#endif
#include "debug.h"

void get_uid_ipv6(ipv6_addr_t *addr, uniqueid_mode_t mode) {
    ipv6_addr_t header = {
        .u8 = {0},
    };
    switch (mode) {
    case UNIQUEID_STATIC_MODE:
        ipv6_addr_from_str(&header, CONFIG_HEADER_ADDRESS_ID);
        memcpy(addr->u8, header.u8, 8);
        char addr_cpu[CPUID_LEN];
        CPUID(addr_cpu);
        memcpy(addr->u8 + LAST_OCTECTS, addr_cpu, CPUID_LEN <= 8? CPUID_LEN : OCTETS_BYTE_SIZE );
        break;
    case UNIQUEID_RANDOM_MODE:
        ipv6_addr_from_str(&header, CONFIG_HEADER_ADDRESS_ID);
        memcpy(addr->u8, header.u8, OCTETS_BYTE_SIZE);
        union random_buff random_number = {0};
        get_uid_seed(&random_number, OCTETS_BYTE_SIZE);
        memcpy(addr->u8 + LAST_OCTECTS, random_number.u8, OCTETS_BYTE_SIZE);
        break;
    default:
        break;
    }
}

uint32_t get_uid_seed(void *val, const uint8_t len) {
    uint32_t rval;
#if (MODULE_AT86RF2XX || MODULE_AT86RF215)
    int index = get_ieee802154_iface();
    netif_t *iface = netif_get_by_id(index);
    netif_get_opt(iface, NETOPT_RANDOM, 0, &rval, sizeof(uint32_t));
#else
    hwrng_init();
    hwrng_read(&rval, sizeof(rval));
#endif
    for (uint8_t i = 0; i < len; i++) {
        random_init(rval);
        rval = random_uint32();
        memcpy(((uint8_t*)val + i), &rval, sizeof(uint8_t));
    }

    return 0;
}
