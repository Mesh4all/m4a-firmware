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
 *
 * @brief       Board specific implementations for the STM32F469I-DISCO board
 *
 * @author      luisan00 <luisan00@hotmail.com>
 * 
 *
 * @}
 */

#include "board.h"
#include "periph/gpio.h"

#include <stdio.h>

void board_init(void)
{
    /* Initialize the CPU */
    cpu_init();

    /* Initialize the on-board LEDs */
    gpio_init(LED0_PIN, GPIO_OUT);

    gpio_init(LED1_PIN, GPIO_OUT);

    gpio_init(LED2_PIN, GPIO_OUT);

    gpio_init(LED3_PIN, GPIO_OUT);
}