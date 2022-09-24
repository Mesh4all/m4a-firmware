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
#if (MODULE_AT86RF2XX || MODULE_AT86RF215)
#include "net/netdev/ieee802154.h"
#include "net/gnrc.h"
#endif
#if MODULE_PERIPH_HWRNG
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

void get_uid_seed(void *val, uint8_t len, uint8_t uid_mode) {
    uint32_t rval;
    uint8_t cpuid_bytes = CPUID_LEN / 2;
    char addr_cpu[CPUID_LEN];
    CPUID(addr_cpu);
    switch (uid_mode) {
    case UNIQUEID_STATIC_MODE:
        if (len > cpuid_bytes) {
            memcpy(val, addr_cpu, cpuid_bytes);
            len = len - cpuid_bytes;
            return get_uid_seed((uint8_t *)val + cpuid_bytes, len, UNIQUEID_RANDOM_MODE);
        } else {
            memcpy(val, addr_cpu, len);
            break;
        }
    case UNIQUEID_RANDOM_MODE:
        DEBUG("rand mode\n");
#if (MODULE_AT86RF2XX || MODULE_AT86RF215)
        netdev_type_t at_ifaces[] = {NETDEV_AT86RF2XX, NETDEV_AT86RF215};
        gnrc_netif_t *iface = NULL;
        for (uint8_t i = 0; i < sizeof(at_ifaces); i++) {
            if ((iface = gnrc_netif_get_by_type(at_ifaces[i], NETDEV_INDEX_ANY)) != NULL) {
                break;
            }
        }
        netif_get_opt(&iface->netif, NETOPT_RANDOM, 0, &rval, sizeof(uint32_t));
#else
        hwrng_init();
        hwrng_read(&rval, sizeof(rval));
#endif
        for (uint8_t i = 0; i < len; i++) {
            random_init(rval);
            rval = random_uint32();
            memcpy(((uint8_t *)val + i), &rval, sizeof(uint8_t));
        }
        break;
    default:
        break;
    }
    return;
}
