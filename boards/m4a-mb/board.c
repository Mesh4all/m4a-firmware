/* 
 * Copyright (c) 2021 Mesh4all <mesh4all.org>
 
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
 * @ingroup         boards_m4a_mb
 * @brief           Board specific implementations for M4A-MB
 * @author          luisan00 <luis@hotmail.com>
 */
#include "board.h"
#include "periph/gpio.h"

void board_init(void)
{
    // initialize the on-board Orange D0 LED
    gpio_init(LED0_PIN, GPIO_OUT);
    LED0_OFF;

    // initialize the on-board Green D1 LED
    gpio_init(LED1_PIN, GPIO_OUT);
    gpio_clear(LED1_PIN);

    // initialize the on-board Yellow D2 LED
    gpio_init(LED2_PIN, GPIO_OUT);
    gpio_clear(LED2_PIN);

    // initialize the on-board user-button
    gpio_init(BTN0_PIN, BTN0_MODE);

    // initialize the CPU
    cpu_init();
}
