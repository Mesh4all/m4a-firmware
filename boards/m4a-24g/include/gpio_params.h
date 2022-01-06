/*
 * Copyright (C) 2015 Freie Universität Berlin
 * Copyright (C) 2015 Kaspar Schleiser <kaspar@schleiser.de>
 * Copyright (C) 2021 Mesh4all <contact@mesh4all.org>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
/**
 * @ingroup boards_m4a-24g
 * @{
 *
 * @file
 * @brief   Board specific configuration of direct mapped GPIOs
 *
 * @author  Hauke Petersen <hauke.petersen@fu-berlin.de>
 * @author  Kaspar Schleiser <kaspar@schleiser.de>
 * @author  Sebastian Meiling <s@mlng.net>
 * @author  Luis A. Ruiz <luisan00@hotmail.com>
 */

#ifndef GPIO_PARAMS_H
#define GPIO_PARAMS_H

#include "board.h"
#include "saul/periph.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ## Saul GPIO params
 *
 * @brief Saul Settings
 * @see [[S]ensor [A]ctuator [U]ber [L]ayer](https://doc.riot-os.org/group__drivers__saul.html)
 *
 * ### LEDs and Buttons
 *
 * | index | name          | pin      | mode      | flags              |
 * |-------|---------------|----------|-----------|--------------------|
 * | 0     | "LED(orange)" | LED0_PIN | GPIO_OUT  | SAUL_GPIO_INVERTED |
 * | 1     | "Button(SW0)" | BTN0_PIN | BTN0_MODE | SAUL_GPIO_INVERTED |
 */
static const  saul_gpio_params_t saul_gpio_params[] =
{
    {
        .name = "LED(orange)",
        .pin = LED0_PIN,
        .mode = GPIO_OUT,
        .flags = SAUL_GPIO_INVERTED,
    },
    {
        .name = "Button(SW0)",
        .pin  = BTN0_PIN,
        .mode = BTN0_MODE,
        .flags = SAUL_GPIO_INVERTED,
    },
};

#ifdef __cplusplus
}
#endif

#endif /* GPIO_PARAMS_H */
/** @} */
