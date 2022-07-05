/*
 * Copyright (C) 2017 DAI Labor Technische Universität Berlin
 * Copyright (C) 2022 Mesh4all <contact@mesh4all.org>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup   boards_meshme
 * @{
 *
 * @file
 * @brief     Board specific configuration of direct mapped GPIOs
 *
 * @author    Anon Mall <anon.mall@gt-arc.com>
 */

#ifndef GPIO_PARAMS_H
#define GPIO_PARAMS_H

#include "board.h"
#include "saul/periph.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    GPIO pin configuration
 */
static const saul_gpio_params_t saul_gpio_params[] = {
    {
        .name = "LED0 (?)",
        .pin = LED0_PIN,
        .mode = GPIO_OUT,
        .flags = SAUL_GPIO_INIT_CLEAR,
    },
    {
        .name = "LED1 (Green)",
        .pin = LED1_PIN,
        .mode = GPIO_OUT,
        .flags = SAUL_GPIO_INIT_CLEAR,
    },
    {
        .name = "LED2 (Orange)",
        .pin = LED2_PIN,
        .mode = GPIO_OUT,
        .flags = SAUL_GPIO_INIT_CLEAR,
    },
    {
        .name = "LED3 (Red)",
        .pin = LED3_PIN,
        .mode = GPIO_OUT,
        .flags = SAUL_GPIO_INIT_CLEAR,
    },
    {
        .name = "SW0/K1 button",
        .pin = BTN0_PIN,
        .mode = BTN0_MODE,
    },
    {
        .name = "SW1/K2 button",
        .pin = BTN1_PIN,
        .mode = BTN1_MODE,
    },
};

#ifdef __cplusplus
}
#endif

#endif /* GPIO_PARAMS_H */
/** @} */
