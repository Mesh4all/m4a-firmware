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
 * @author  eduazocar <eduazocarv@gmail.com>
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "embUnit.h"
#include "uniqueid.h"
#include "random.h"
#include "unique_random.h"

#if (MODULE_AT86RF2XX || MODULE_AT86RF215)
#include "radio.h"
#endif
#define BIT_ON_MASK (0x01)

void random_generator(uint8_t bit_ref) {

#if (MODULE_AT86RF2XX || MODULE_AT86RF215)
    int index = get_ieee802154_iface();
    netif_t *iface = netif_get_by_id(index);
#endif
    uint8_t avg_bit32 = bit_ref / 32;
    if (bit_ref % 32 != 0) {
        avg_bit32++;
    }
    uint32_t random_byte_block = 0;
    for (uint8_t i = 0; i < bit_ref; i++) {
        printf("ref %u", (uint8_t)(i + 1));
        for (uint8_t j = 0; j < avg_bit32; j++) {
#if (MODULE_AT86RF2XX || MODULE_AT86RF215)
            netif_get_opt(iface, NETOPT_RANDOM, 0, &random_byte_block, sizeof(uint32_t));
#endif
            random_init(random_byte_block);
            random_byte_block = random_uint32();
            printf(" Block: %d\t%" PRIx32 " \t", j, random_byte_block);
            print_binary(random_byte_block, bit_ref / avg_bit32);
            printf("\t");
        }
        printf("\n");
    }
}

void print_binary(uint32_t val, uint8_t len) {
    for (int16_t i = len - 1; i >= 0; i--) {
        printf("%u", (uint8_t)((val >> i) & BIT_ON_MASK));
    }
}
