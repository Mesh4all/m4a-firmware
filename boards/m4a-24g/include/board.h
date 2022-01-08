/*
 * Copyright (C) 2014 Freie Universität Berlin
 * Copyright (C) 2021 Mesh4all <contact@mesh4all-org>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
/**
 * @ingroup     boards_m4a-24g
 * @{
 *
 * @file
 * @brief       Board specific definitions for the M4A-24G board
 *
 * @author      Thomas Eichinger <thomas.eichinger@fu-berlin.de>
 * @author      Sebastian Meiling <s@mlng.net>
 * @author      luisan00 <luisan00@hotmail.com>
 */

#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"
#include "periph_conf.h"
#include "periph_cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    xtimer configuration
 * @{
 */
#define XTIMER_DEV          TIMER_DEV(1)    /*!< xtimer device */
#define XTIMER_CHAN         (0)             /*!< xtimer channel */
/** @} */

/**
 * @name    ztimer configuration
 * @{
 */
#define CONFIG_ZTIMER_USEC_TYPE    ZTIMER_TYPE_PERIPH_TIMER /*!< ztimer periph */
#define CONFIG_ZTIMER_USEC_DEV     TIMER_DEV(1)             /*!< ztimer device */
/**
 * @note timer_set() may underflow for values smaller than 9, set 10 as margin
 */
#define CONFIG_ZTIMER_USEC_MIN     (10)
/** @} */

/**
 * @name    AT86RF233 configuration
 * @{
 */
#define AT86RF2XX_PARAM_CS         GPIO_PIN(PB, 31) /*!< CS pin routed to PB31 in the MCU */
#define AT86RF2XX_PARAM_INT        GPIO_PIN(PB, 0)  /*!< INT pin routed to PB0 in the MCU */
#define AT86RF2XX_PARAM_SLEEP      GPIO_PIN(PA, 20) /*!< SLEEP pin routed to PA20 in the MCU */
#define AT86RF2XX_PARAM_RESET      GPIO_PIN(PB, 15) /*!< RST is routed to PB15 in the MCU */
/** @} */

/**
 * @name    LED pin definitions and handlers
 * @{
 */
#define LED0_PIN            GPIO_PIN(0, 19)                   /*!< LED0 is routed to GPIO(0, 19)*/
#define LED_PORT            PORT->Group[0]                    /*!< LED0 port*/
#define LED0_MASK           (1 << 19)                         /*!< LED0 mask */
#define LED0_ON             (LED_PORT.OUTCLR.reg = LED0_MASK) /*!< LED0 Handler */
#define LED0_OFF            (LED_PORT.OUTSET.reg = LED0_MASK) /*!< LED0 Handler */
#define LED0_TOGGLE         (LED_PORT.OUTTGL.reg = LED0_MASK) /*!< LED0 Handler */
/** @} */

/**
 * @name    SW0 (Button) pin definitions
 * @{
 */
#define BTN0_PORT           PORT->Group[0]      /*!< BTN0 port */
#define BTN0_PIN            GPIO_PIN(0, 28)     /*!< BTN0 routed to GPIO_PIN(0, 28) */
#define BTN0_MODE           GPIO_IN_PU          /*!< BTN0 GPIO mode */
/** @} */

/**
 * @name    Antenna configuration pin interface
 * @{
 */
#define RFCTL1_PIN          GPIO_PIN(0, 9)  /*!< RF control pin 1 assigned to GPIO(0, 9) */
#define RFCTL2_PIN          GPIO_PIN(0, 12) /*!< RF control pin 2 is assigned to GPIO(0, 12) */
/** @} */

/**
 * @brief   Antenna configuration values
 *
 * - RFCTL_ANTENNA_BOARD: On-board antenna
 * - RFCTL_ANTENA_EXT: External antenna
 */
enum {
    RFCTL_ANTENNA_BOARD,
    RFCTL_ANTENNA_EXT,
};

/**
 * @name    Default antenna configuration
 * @{
 */
#ifndef RFCTL_ANTENNA_DEFAULT
/**
 * @brief **IF NOT** defined as default any RF antenna option, use the on-board antenna
 */
#define RFCTL_ANTENNA_DEFAULT      RFCTL_ANTENNA_BOARD
#endif
/** @} */

/**
 * @brief   Initialize board specific hardware, including clock, LEDs and std-IO
 */
void board_init(void);

/**
 * @brief   Set antenna switch
 *
 * @param [in] antenna antenna to be used
 *
 * | Option              | Value | Description      |
 * |:--------------------|:-----:|:-----------------|
 * | RFCTL_ANTENNA_BOARD | 0     | On-board antenna |
 * | RFCTL_ANTENNA_EXT   | 1     | External antenna |
 */
void board_antenna_config(uint8_t antenna);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
