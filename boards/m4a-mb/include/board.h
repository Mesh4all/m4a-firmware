/*
 * Copyright (C) 2021 Mesh For All <mesh4all.org>
 *
 * This file is is licensed under the Apache License, Version 2.0,
 * you may not use this file except in compliance with the License.
 * See the LICENSE file in the root folder for more information
 */

/**
 * @ingroup     boards_m4a_mb
 * @{
 *
 * @file
 * @brief       Board specific definitions for the M4A-MB
 *
 * @author      luisan00 <luisan00@hotmail.com>
 */

#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"
#include "periph_conf.h"
#include "periph_cpu.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief    xtimer configuration
 * 
 */
#define XTIMER_DEV TIMER_DEV(1)
#define XTIMER_CHAN (0)

/**
 * @brief   LEDs pin definitions and handlers
 *
 */
#define LED_PORT PORT->Group[PB]

/**
 * @brief   LED-D0 (orange) definition
 * 
 */
#define LED0_PIN GPIO_PIN(PB, 30)
#define LED0_MASK (1 << 30)

/**
 * @brief   LED-D0 handler
 * 
 */
#define LED0_ON (LED_PORT.OUTCLR.reg = LED0_MASK)
#define LED0_OFF (LED_PORT.OUTSET.reg = LED0_MASK)
#define LED0_TOGGLE (LED_PORT.OUTTGL.reg = LED0_MASK)

/**
 * @brief   LED-D1 (green) definition
 * 
 */
#define LED1_PIN GPIO_PIN(PB, 7)
#define LED1_MASK (1 << 7)

/**
 * @brief   LED-D1 handler
 * 
 */
#define LED1_ON (LED_PORT.OUTCLR.reg = LED1_MASK)
#define LED1_OFF (LED_PORT.OUTSET.reg = LED1_MASK)
#define LED1_TOGGLE (LED_PORT.OUTTGL.reg = LED1_MASK)

/**
 * @brief   LED-D2 (yellow) definition
 * 
 */
#define LED2_PIN GPIO_PIN(PB, 6)
#define LED2_MASK (1 << 6)
/**
 * @brief   LED-D2 handler
 * 
 */
#define LED2_ON (LED_PORT.OUTCLR.reg = LED2_MASK)
#define LED2_OFF (LED_PORT.OUTSET.reg = LED2_MASK)
#define LED2_TOGGLE (LED_PORT.OUTTGL.reg = LED2_MASK)

/**
 * @brief   SW0 (Button) pin definition
 * 
 */
#define BTN0_PORT PORT->Group[PA]
#define BTN0_PIN GPIO_PIN(PA, 15)
#define BTN0_MODE GPIO_IN_PU

/**
 * @brief   AT86RF215 pin definitions
 *
 */
#define AT86RF215_PARAM_SPI SPI_DEV(0)
#define AT86RF215_PARAM_CS GPIO_PIN(PA, 5)    /* PORTA-05 */
#define AT86RF215_PARAM_INT GPIO_PIN(PB, 4)   /* PORTB-04 */
#define AT86RF215_PARAM_RESET GPIO_PIN(PB, 2) /* PORTB-02 */

    void board_init(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // BOARD_H