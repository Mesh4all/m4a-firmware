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
void subnet_to_ipv6(ipv6_addr_t *addr) {

#ifdef CONFIG_MODE_STATIC
    ipv6_addr_t header = {
        .u8 = {0},
    };
    char addr_cpu[CPUID_LEN] = {0};
    CPUID(addr_cpu);
    ipv6_addr_from_str(&header, CONFIG_HEADER_ADDRESS_ID);
    memcpy((char *)addr->u8, (char *)header.u8, 4);
    strncat((char *)addr->u8, addr_cpu, 4);

#endif

#ifdef CONFIG_MODE_RANDOM
    ipv6_addr_t header = {
        .u8 = {0},
    };
    union random_buff random_number;
    ipv6_addr_from_str(&header, CONFIG_HEADER_ADDRESS_ID);
    memcpy((char *)addr->u8, (char *)header.u8, 4);
#if (MODULE_AT86RF2XX || MODULE_AT86RF215)
    int index = get_ieee802154_iface();
    netif_t *iface = netif_get_by_id(index);
    netif_get_opt(iface, NETOPT_RANDOM, 0, &random_number, sizeof(random_number));
#else
    hwrng_init();
    hwrng_read(&random_number, sizeof(random_number));
#endif
    random_init(random_number.u32);
    random_number.u32 = random_uint32();
    strncat((char *)addr->u8, (char *)random_number.u8, 4);
#endif

#ifdef CONFIG_MODE_MANUAL
    ipv6_addr_t header = {
        .u8 = {0},
    };
    ipv6_addr_t subnet = {
        .u8 = {0},
    };
    ipv6_addr_from_str(&header, CONFIG_HEADER_ADDRESS_ID);
    ipv6_addr_from_str(&subnet, CONFIG_SUBNET_ADDRESS_ID);
    memcpy((char *)addr->u8, (char *)header.u8, 4);
    memcpy((char *)addr->u8 + 4, (char *)subnet.u8, 4);

#endif
}
