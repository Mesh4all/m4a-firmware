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
 * @brief       Storage_initialization_setup
 *
 * @author      luisan00 <luisan00@hotmail.com>
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "storage.h"
#include "default_params.h"

#if MODULE_RPL_PROTOCOL
#include "rpl_protocol.h"
int set_rpl_params(storage_data_t data);
#endif
#if MODULE_RADIO
#include "radio.h"
int set_radio_params(storage_data_t data);
#endif

void load_settings(void) {
    storage_data_t data;
    if (mtd_dump() < 0) {
        printf("Is not there settings parameters saved");
        return;
    }
    mtd_load(&data, sizeof(data));
#if MODULE_RADIO
    if (set_radio_params(data) < 0) {
        printf("Radio params wrong set\n");
    }
#endif
#if MODULE_RPL_PROTOCOL
    if (set_rpl_params(data) < 0) {
        printf("RPL params wrong set\n");
    }
#endif
    return;
}

#if MODULE_RADIO
int set_radio_params(storage_data_t data) {
    if (set_netopt_tx_power(data.radio_tx_power) < 0) {
        return -1;
    }
    if (set_netopt_channel(data.channel) < 0) {
        return -1;
    }
    return 0;
}
#endif

#if MODULE_RPL_PROTOCOL
int set_rpl_params(storage_data_t data) {
    rpl_setup(data.rpl_mode);
    return 0;
}
#endif
