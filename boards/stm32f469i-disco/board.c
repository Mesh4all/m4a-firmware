/*
 * Copyright (C) 2021 Mesh4all <mesh4all.org>
 *
 * This file is is licensed under the Apache License, Version 2.0,
 * you may not use this file except in compliance with the License.
 * See the LICENSE file in the root folder for more information
 */

/**
 *
 * @brief       Board specific implementations for the STM32F469I-DISCO board
 *
 * @author      Vincent Dupont <vincent@otakeys.com>
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