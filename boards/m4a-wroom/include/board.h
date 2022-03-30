/*
 * Copyright (C) 2018 Gunar Schorcht
 * Copyright (C) 2022 Mesh4all <mesh4all.org>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_m4a-wroom
 * @brief       Board specific definitions for m4a-wroom board
 * @{
 *
 * @file
 * @author      Gunar Schorcht <gunar@schorcht.net>
 * @author      Eduardo Azócar <eduardo@turpialdev.com>
 */

#ifndef BOARD_H
#define BOARD_H
#include <stdint.h>
#include "cpu.h"
#include "periph_conf.h"
#include "periph_cpu.h"
#include "periph/cpuid.h"

/**
 * @name    Button pin definitions
 * @{
 */

#define BTN0_PIN        GPIO0   /*!< Default button pin definition */
#define BTN0_MODE       GPIO_IN /*!< Default button mode */

#ifndef BTN0_INT_FLANK
#define BTN0_INT_FLANK  GPIO_FALLING    /*!< Default interrupt flank definition for the button */
#endif

#define BUTTON0_PIN     BTN0_PIN        /*!< Compatibility with previous versions */

/** @} */

/**
 * @name    DS18B20 temperature Sensor
 * @{
 */

/**
 * @brief   Default DS18B20 GPIO pin definition to get ds18 required params
 *
 */
#ifndef DS18B20_PIN
#define DS18B20_PIN         GPIO14          /*!< DS18 pin definition */
#endif

#ifndef DS18B20_MODE_OUT
#define DS18B20_MODE_OUT    GPIO_OD_PU      /*!< DS18 pin mode_out */
#endif

#ifndef DS18B20_MODE_IN
#define DS18B20_MODE_IN    GPIO_IN         /*!< DS18 pin mode_input */
#endif

/** @} */

/**
 * @name    Soil Moisture Analog Sensor
 * @{
 */

/**
 * @brief   Default Soil Moisture Analog Sensor GPIO pin definition
 */

#define MS_HW360_PIN        GPIO12  /*!<Default GPIO pin definition */
/** @} */

/* include common board definitions as last step */
#include "board_common.h"

/**
  * @brief  Get the CPU unique ID of this platform
  * This function gets base cpu address using cpuid_get
  *
  * @param  id  pointer to char array with CPUID_LEN size.
  *
  */
#define CPUID(id)  cpuid_get(id);
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the board specific hardware
 */
static inline void board_init(void) {
    /* there is nothing special to initialize on this board */
    board_init_common();
}

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif /* BOARD_H */
/** @} */
