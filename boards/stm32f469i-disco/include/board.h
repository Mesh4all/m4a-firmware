/*
 * Copyright (C) 2021 Mesh4all <mesh4all.org>
 *
 * This file is is licensed under the Apache License, Version 2.0,
 * you may not use this file except in compliance with the License.
 * See the LICENSE file in the root folder for more information
 */

/**
 * @brief       Board specific definitions for the STM32F469I-DISCO board
 * 
 * @author      luisan00 <luisan00@hotmail.com>
 *
 */

#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"
#include "periph_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    LEDs definitions and handlers.
 * 
 * @brief   Four on-board LEDs are availoable as follows
 *          - LD0 PORT_G, 6 Green
 *          - LD1 PORT_D, 4 Orange
 *          - LD2 PORT_D, 5 Red
 *          - LD3 PORT_K, 3 Blue
 * 
 */
#define LED0_PIN            GPIO_PIN(PORT_G, 6)
#define LED1_PIN            GPIO_PIN(PORT_D, 4)
#define LED2_PIN            GPIO_PIN(PORT_D, 5)
#define LED3_PIN            GPIO_PIN(PORT_K, 3)
//
#define LED0_PORT           GPIOG
#define LED1_PORT           GPIOD
#define LED2_PORT           GPIOD
#define LED3_PORT           GPIOK
//
#define LED0_MASK           (1 << 6)
#define LED1_MASK           (1 << 4)
#define LED2_MASK           (1 << 5)
#define LED3_MASK           (1 << 3)
//
#define LED0_ON             (LED0_PORT->BSRR = LED0_MASK)
#define LED0_OFF            (LED0_PORT->BSRR = (LED0_MASK << 16))
#define LED0_TOGGLE         (LED0_PORT->ODR  ^= LED0_MASK)
//
#define LED1_ON             (LED1_PORT->BSRR = LED1_MASK)
#define LED1_OFF            (LED1_PORT->BSRR = (LED1_MASK << 16))
#define LED1_TOGGLE         (LED1_PORT->ODR  ^= LED1_MASK)
//
#define LED2_ON             (LED2_PORT->BSRR = LED2_MASK)
#define LED2_OFF            (LED2_PORT->BSRR = (LED2_MASK << 16))
#define LED2_TOGGLE         (LED2_PORT->ODR  ^= LED2_MASK)
//
#define LED3_ON             (LED3_PORT->BSRR = LED3_MASK)
#define LED3_OFF            (LED3_PORT->BSRR = (LED3_MASK << 16))
#define LED3_TOGGLE         (LED3_PORT->ODR  ^= LED3_MASK)


/**
 * @brief The blue button B2 located bottom side is available to be used as a digital input or as
 * alternate function Wake-up. When the button is depressed the logic state is 1, otherwise the
 * logic state is 0.
 * 
 */
#define BTN0_PIN            GPIO_PIN(PORT_A, 0)
#define BTN0_MODE           GPIO_IN

/**
 * @brief Initialize board specific hardware, including clock, LEDs and std-IO
 */
void board_init(void);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // BOARD_H