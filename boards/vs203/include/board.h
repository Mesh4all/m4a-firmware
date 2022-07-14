/*
 * Copyright (C) 2014 Loci Controls Inc.
 * Copyright (C) 2022 Mesh4all <contact@mesh4all.org>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_vs203
 * @{
 *
 * @file
 * @brief       Board specific definitions for the VS203 board
 *
 * @author      Ian Martin <ian@locicontrols.com>
 * @author      Luis A. Ruiz <luisan00@hotmail.com>
 */
#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"
#include "periph/gpio.h"
#include "cc2538_eui_primary.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    LED pin definitions and handlers
 * @{
 */
#define LED0_PIN GPIO_PIN(PORT_B, 1)      /*!< LED0 () routed to PB1 pin */
#define LED0_ON gpio_set(LED0_PIN)        /*!< LED0 On handler */
#define LED0_OFF gpio_clear(LED0_PIN)     /*!< LED0 Off handler */
#define LED0_TOGGLE gpio_toggle(LED0_PIN) /*!< LED0 Toggle handler */

#define LED1_PIN GPIO_PIN(PORT_B, 2)      /*!< LED1 () routed to PB2 pin */
#define LED1_ON gpio_set(LED1_PIN)        /*!< LED1 On handler */
#define LED1_OFF gpio_clear(LED1_PIN)     /*!< LED1 Off handler */
#define LED1_TOGGLE gpio_toggle(LED1_PIN) /*!< LED1 Toggle handler */

#define LED2_PIN GPIO_PIN(PORT_B, 3)      /*!< LED2 () routed to PB3 pin */
#define LED2_ON gpio_set(LED2_PIN)        /*!< LED2 On handler */
#define LED2_OFF gpio_clear(LED2_PIN)     /*!< LED2 Off handler */
#define LED2_TOGGLE gpio_toggle(LED2_PIN) /*!< LED2 Toggle handler */

#define LED3_PIN GPIO_PIN(PORT_B, 4)      /*!< LED3 () routed to PB4 pin */
#define LED3_ON gpio_set(LED3_PIN)        /*!< LED3 On handler */
#define LED3_OFF gpio_clear(LED3_PIN)     /*!< LED3 Off handler */
#define LED3_TOGGLE gpio_toggle(LED3_PIN) /*!< LED3 Toggle handler */
/** @} */

/**
 * @name User button SW0 pin definition, labeled K1 on the board
 * @{
 */
#define BTN0_PIN GPIO_PIN(PORT_C, 7) /*!< It's routed to PC7 pin */
#define BTN0_MODE GPIO_IN_PU         /*!< Pin mode */
/** @} */

/**
 * @name Flash Customer Configuration Area (CCA) parameters
 * @{
 */
#ifndef UPDATE_CCA
#define UPDATE_CCA 1 /*!< On/Off Customer Configuration Area should be updated */
#endif

#define CCA_BACKDOOR_ENABLE 1       /*!< On/Off the backdoor on boot */
#define CCA_BACKDOOR_PORT_A_PIN 7   /*!< Boot button is PORT_A, 7 (PA7) */
#define CCA_BACKDOOR_ACTIVE_LEVEL 0 /*!< Active when signal is low: 0 */
/** @} */

/**
 * @name    xtimer configuration
 * @{
 */
#define XTIMER_WIDTH (16)       /*!< ToDo */
#define XTIMER_BACKOFF (50)     /*!< ToDo */
#define XTIMER_ISR_BACKOFF (40) /*!< ToDo */
/** @} */

/**
 * @name   RF Front-End configuration
 *
 * @brief The FE CC2592 is controlled by the CC2538 with 3 pins, both are connected as follow:
 *
 * | CC2538 (MCU) | CC2592 (FE) | Description                            |
 * |--------------|-------------|----------------------------------------|
 * | PORT_D, 2    | HGM_EN      | High Gain Mode (enabled/disabled)      |
 * | PORT_C, 2    | LNA_EN      | Low Noise Amplifier (enabled/disabled) |
 * | PORT_C, 3    | PA_EN       | Power Amplifier (enabled/disabled)     |
 *
 * The CC2538 RF Core needs to use "observable signals" to drive the LNA_EN and the PA_EN pins
 * @{
 */
#define CONFIG_CC2538_RF_OBS_SIG_0_PCX 3 /*!< tx active to PC3, it controls the PA */
#define CONFIG_CC2538_RF_OBS_SIG_1_PCX 2 /*!< rx active to PC2, it controls the LNA */
#define CONFIG_CC2538_RF_OBS_SIG_2_PCX 0 /*!< rssi valid, routed to the on board red LED PC0 */

#define HGM_PIN GPIO_PIN(PORT_D, 2)     /*!< High Gain Mode (HGM) pin */
#define HGM_ON gpio_set(HGM_PIN)        /*!< HGM on handler */
#define HGM_OFF gpio_clear(HGM_PIN)     /*!< HGM off handler */
#define HGM_TOGGLE gpio_toggle(HGM_PIN) /*!< HGM toggle handler */
/** @} */

/**
 * @brief   Initialize board specific hardware, including clock, LEDs and std-IO
 */
void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
