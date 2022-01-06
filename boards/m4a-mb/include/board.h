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
 * @ingroup     boards_m4a-mb
 * @{
 * @file
 * @brief       Board specific definitions for the M4A-MB
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
 * @name   xTimer configuration
 * @{
 */
#define XTIMER_DEV TIMER_DEV(1)                         /*!< Timer Device */
#define XTIMER_CHAN (0)                                 /*!< Timer Channel */
/** @} */

/**
 * @name LED PORT is PB
 * @{
 */
#define LED_PORT PORT->Group[PB]                        /*!< Group is Port B */
/** @} */

/**
 * @name   LED D0 (orange) definition and handler
 * @{
 */
#define LED0_PIN GPIO_PIN(PB, 30)                       /*!< LED0 is routed to pin PB30 in the MCU */
#define LED0_MASK (1 << 30)                             /*!< --- */
#define LED0_ON (LED_PORT.OUTCLR.reg = LED0_MASK)       /*!< Power On LED0 */
#define LED0_OFF (LED_PORT.OUTSET.reg = LED0_MASK)      /*!< Power Off LED0 */
#define LED0_TOGGLE (LED_PORT.OUTTGL.reg = LED0_MASK)   /*!< Change the status of LED0 */
/** @} */

/**
 * @name   LED-D1 (green) definition and handler
 * @{
 */
#define LED1_PIN GPIO_PIN(PB, 7)                        /*!< LED1 is routed to pin PB7 in the MCU */
#define LED1_MASK (1 << 7)                              /*!<  --- */
#define LED1_ON (LED_PORT.OUTCLR.reg = LED1_MASK)       /*!< Power On LED1 */
#define LED1_OFF (LED_PORT.OUTSET.reg = LED1_MASK)      /*!< Power Off LED1 */
#define LED1_TOGGLE (LED_PORT.OUTTGL.reg = LED1_MASK)   /*!< Change the status of LED1 */
/** @} */

/**
 * @name   LED-D2 (yellow) definition and handler
 * @{
 */
#define LED2_PIN GPIO_PIN(PB, 6)                        /*!< LED2 is routed to pin PB6 in the MCU */
#define LED2_MASK (1 << 6)                              /*!<  --- */
#define LED2_ON (LED_PORT.OUTCLR.reg = LED2_MASK)       /*!< Power On LED2 */
#define LED2_OFF (LED_PORT.OUTSET.reg = LED2_MASK)      /*!< Power Off LED2 */
#define LED2_TOGGLE (LED_PORT.OUTTGL.reg = LED2_MASK)   /*!< Change the status of LED2 */
/** @} */

/**
 * @name   SW0 (Button) pin definition
 * @{
 */
#define BTN0_PORT PORT->Group[PA]                       /*!< --- */
#define BTN0_PIN GPIO_PIN(PA, 15)                       /*!< --- */
#define BTN0_MODE GPIO_IN_PU                            /*!< --- */
/** @} */

/**
 * @name   AT86RF215 pin definitions
 * @{
 */
#define AT86RF215_PARAM_SPI SPI_DEV(0)          /*!< Use SPI 0 */
#define AT86RF215_PARAM_CS GPIO_PIN(PA, 5)      /*!< CS is routef to PA5 in the MCU */
#define AT86RF215_PARAM_INT GPIO_PIN(PB, 4)     /*!< IN is routed to PB4 in the MCU */
#define AT86RF215_PARAM_RESET GPIO_PIN(PB, 2)   /*!< RESET is routed to PB2 in the MCU */
/** @} */

/**
 * @brief Initialize the board
 */
void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
