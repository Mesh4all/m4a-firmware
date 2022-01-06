/*
 * Copyright (C) 2021 Mesh4all <contact@mesh4all.org>
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
/**
 * @ingroup     boards_m4a-24g
 * @{
 *
 * @file
 * @brief       Board specific implementations for the M4A-24G board
 *
 * @author      Luis A. Ruiz <luisan00@hotmail.com>
 * @author      Thomas Eichinger <thomas.eichinger@fu-berlin.de>
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include "board.h"
#include "periph/gpio.h"

void board_antenna_config(uint8_t antenna)
{
    if (antenna == RFCTL_ANTENNA_EXT)
    {
        gpio_set(RFCTL1_PIN);
        gpio_clear(RFCTL2_PIN);
    }
    else if (antenna == RFCTL_ANTENNA_BOARD)
    {
        gpio_clear(RFCTL1_PIN);
        gpio_set(RFCTL2_PIN);
    }
}

void board_init(void)
{
    /* initialize the on-board LED */
    gpio_init(LED0_PIN, GPIO_OUT);
    LED0_OFF;

    /* initialize the on-board antenna switch */
    if (IS_USED(MODULE_AT86RF233))
    {
        gpio_init(RFCTL1_PIN, GPIO_OUT);
        gpio_init(RFCTL2_PIN, GPIO_OUT);

        /* set default antenna switch configuration */
        board_antenna_config(RFCTL_ANTENNA_DEFAULT);
    }
}
