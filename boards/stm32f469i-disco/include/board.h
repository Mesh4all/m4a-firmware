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
 */

#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"
#include "periph_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Macros for controlling the on-board LEDs.
 * @{
 */
#define LED0_PIN            GPIO_PIN(PORT_J, 13)
#define LED1_PIN            GPIO_PIN(PORT_J, 5)
#define LED2_PIN            GPIO_PIN(PORT_A, 12)
#define LED3_PIN            GPIO_PIN(PORT_D, 4)
//
#define LED0_PORT           GPIOJ
#define LED1_PORT           GPIOJ
#define LED2_PORT           GPIOA
#define LED3_PORT           GPIOD
//
#define LED0_MASK           (1 << 13)
#define LED1_MASK           (1 << 5)
#define LED2_MASK           (1 << 12)
#define LED3_MASK           (1 << 4)
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
 * @name User button
 */
#define BTN0_PIN            GPIO_PIN(PORT_A, 0)
#define BTN0_MODE           GPIO_IN
/** @} */

/**
 * @brief Initialize board specific hardware, including clock, LEDs and std-IO
 */
void board_init(void);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // BOARD_H