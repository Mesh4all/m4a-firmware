/*
 * Copyright (C) 2017 Travis Griggs <travisgriggs@gmail.com>
 * Copyright (C) 2017 Dan Evans <photonthunder@gmail.com>
 * Copyright (C) 2021 Mesh4all <mesh4all.org>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup   boards_m4a-mb
 * @{
 * @file
 * @brief     Board specific configuration of direct mapped GPIOs
 */
#ifndef GPIO_PARAMS_H
#define GPIO_PARAMS_H

#include "board.h"
#include "saul/periph.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * ## Saul GPIO params
 *
 * @brief Saul Settings
 * @see [[S]ensor [A]ctuator [U]ber [L]ayer](https://doc.riot-os.org/group__drivers__saul.html)
 *
 * ### LED0
 *
 * @param name "LED (Orange)"
 * @param pin LED0_PIN
 * @param mode GPIO_OUT
 *
 * ### LED1
 *
 * @param name "LED (Green)"
 * @param pin LED1_PIN
 * @param mode SAUL_GPIO_INVERTED
 */
static const saul_gpio_params_t saul_gpio_params[] = {
    {
        .name = "LED (Orange)",
        .pin = LED0_PIN,
        .mode = GPIO_OUT,
    },
    {
        .name = "LED (Green)",
        .pin = LED1_PIN,
        .mode = GPIO_OUT,
        .flags = SAUL_GPIO_INVERTED,
    },
    {
        .name = "LED (Yellow)",
        .pin = LED2_PIN,
        .mode = GPIO_OUT,
        .flags = SAUL_GPIO_INVERTED,
    },
    {
        .name = "Button (SW0)",
        .pin = BTN0_PIN,
        .mode = BTN0_MODE,
    },
};

#ifdef __cplusplus
}
#endif

#endif /* GPIO_PARAMS_H */
/** @} */
