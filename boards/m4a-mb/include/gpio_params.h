/*
 * Copyright (C) 2021 Mesh For All <mesh4all.org>
 *
 * This file is is licensed under the Apache License, Version 2.0,
 * you may not use this file except in compliance with the License.
 * See the LICENSE file in the root folder for more information
 */

/**
 * @ingroup   boards_m4a-mb

 * @brief     Board specific configuration of direct mapped GPIOs
 *
 * @author    luisan00 <luisan00@hotmail.com>
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
    * @brief    GPIO pin configuration
    */
    static const saul_gpio_params_t saul_gpio_params[] =
        {
            {.name = "LED (Orange)",
             .pin = LED0_PIN,
             .mode = GPIO_OUT},
            {.name = "LED (Green)",
             .pin = LED1_PIN,
             .mode = GPIO_OUT,
             .flags = SAUL_GPIO_INVERTED},
            {.name = "LED (Yellow)",
             .pin = LED2_PIN,
             .mode = GPIO_OUT,
             .flags = SAUL_GPIO_INVERTED},
            {.name = "Button (SW0)",
             .pin = BTN0_PIN,
             .mode = BTN0_MODE},
    };

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // GPIO_PARAMS_H