/*
 * Copyright (C) 2021 Mesh4all <mesh4all.org>
 *
 * This file is is licensed under the Apache License, Version 2.0,
 * you may not use this file except in compliance with the License.
 * See the LICENSE file in the root folder for more information
 */

/**
 * @brief       Peripheral MCU configuration for the STM32F769I-DISCO board
 * @author      luisan00 <luisan00@hotmail.com>
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

/*
 *  HSE lock:
 *      By default the HSE clock source of the STM32F469NIH6 is the 8 MHz crystal X2.
 * 
 *  LSE clock:
 *      The 32.768 kHz crystal X3 is the clock source for the embedded RTC.
 */

/* This board provides an LSE */
#ifndef CONFIG_BOARD_HAS_LSE
#define CONFIG_BOARD_HAS_LSE 1
#endif

/* This board provides an HSE */
#ifndef CONFIG_BOARD_HAS_HSE
#define CONFIG_BOARD_HAS_HSE 1
#endif
#define CLOCK_HSE MHZ(8)

#include "periph_cpu.h"
#include "clk_conf.h"
#include "cfg_rtt_default.h"
#include "cfg_timer_tim2.h"
#include "cfg_usb_otg_fs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * USB OTG FS is available on the Micro-AB connector named as CN13
 * with the next pinout:
 * 
 * 1 VBUS (PA9) 
 * 2 D- (PORT_A, 11) 
 * 3 D+ (PORT_A, 12)
 * 4 ID (PORT_A, 10)
 * 5 GND
 * 
 * Virtual COM port is available on the ST-LINK/V2-1 USB Mini-B connector
 * named as CN1
 * 
 * 
 * UART6_Tx: PORT_G, 14
 * UART6_Rx: PORT_G, 9
 * 
 */

/**
 * @name    UART configuration
 * 
 */
    static const uart_conf_t uart_config[] = {
        {.dev = USART1,
         .rcc_mask = RCC_APB2ENR_USART1EN,
         .rx_pin = GPIO_PIN(PORT_B, 10),
         .tx_pin = GPIO_PIN(PORT_B, 11),
         .rx_af = GPIO_AF7,
         .tx_af = GPIO_AF7,
         .bus = APB2,
         .irqn = USART1_IRQn,
#ifdef UART_USE_DMA
         .dma_stream = 4,
         .dma_chan = 4
#endif
        }};

#define UART_0_ISR (isr_usart1)
#define UART_0_DMA_ISR (isr_dma1_stream4)

#define UART_NUMOF ARRAY_SIZE(uart_config)

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // PERIPH_CONF_H