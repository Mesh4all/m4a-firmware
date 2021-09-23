/*
 * Copyright (C) 2021 Mesh4All <mesh4all.org>
 *
 * This file is subject to the terms and conditions of the Apache-2.0 license.
 *  See the file LICENSE in the top level directory for more details.
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
