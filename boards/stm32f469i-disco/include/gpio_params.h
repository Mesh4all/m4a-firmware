/*
 * Copyright (C) 2021 Mesh4all <mesh4all.org>
 *
 * This file is is licensed under the Apache License, Version 2.0,
 * you may not use this file except in compliance with the License.
 * See the LICENSE file in the root folder for more information
 */

/**
 * @brief     Board specific configuration of direct mapped GPIOs
 *
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
static const  saul_gpio_params_t saul_gpio_params[] =
{
    {
        .name = "LD1",
        .pin = LED0_PIN,
        .mode = GPIO_OUT
    },
    {
        .name = "LD2",
        .pin = LED1_PIN,
        .mode = GPIO_OUT
    },
    {
        .name = "LD3",
        .pin = LED2_PIN,
        .mode = GPIO_OUT
    },
    {
        .name = "LD4",
        .pin = LED3_PIN,
        .mode = GPIO_OUT
    },
    {
        .name = "BTN USER",
        .pin  = BTN0_PIN,
        .mode = BTN0_MODE
    },
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // GPIO_PARAMS_H