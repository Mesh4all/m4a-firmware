/*
 * Copyright (c) 2022 Mesh4all <mesh4all.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file
 * @brief           Board specific implementations for the VS203 board
 * @copyright       Copyright (c) 2022 Mesh for All (Mesh4all)
 */
#include "board.h"
#include "cpu.h"
#include "periph/gpio.h"

void board_init(void) {

    // initialize the on-board LED 0
    gpio_init(LED0_PIN, GPIO_OUT);

    // initialize the on-board LED 1
    gpio_init(LED1_PIN, GPIO_OUT);

    // initialize the on-board LED 2
    gpio_init(LED2_PIN, GPIO_OUT);

    // initialize the on-board LED 3
    gpio_init(LED3_PIN, GPIO_OUT);

    // initialize the on-board user-button SW0
    gpio_init(BTN0_PIN, BTN0_MODE);

    // initialize the on-board user-button SW0
    gpio_init(BTN1_PIN, BTN1_MODE);

    if (IS_USED(MODULE_CC2538_RF)) {
        gpio_init(HGM_PIN, GPIO_OUT);
        HGM_ON;
    }
// Compilation error <https://github.com/RIOT-OS/RIOT/pull/16055>
// workaround for lower RIOT versions, <https://github.com/Mesh4all/m4a-firmware/issues/43>
#if RIOT_VERSION_CODE < RIOT_VERSION_NUM(2021, 10, 0, 0)
    cpu_init();
#endif
}
