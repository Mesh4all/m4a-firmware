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

/* This board provides a LSE */
#ifndef CONFIG_BOARD_HAS_LSE
#define CONFIG_BOARD_HAS_LSE 1
#endif

/* This board provides a HSE */
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

  /**
    * @name    UART configuration
    * 
    */
  static const uart_conf_t uart_config[] = {
      // CN1, VCP
      {.dev = USART3,
       .rcc_mask = RCC_APB1ENR_USART3EN,
       .rx_pin = GPIO_PIN(PORT_B, 11),
       .tx_pin = GPIO_PIN(PORT_B, 10),
       .rx_af = GPIO_AF7,
       .tx_af = GPIO_AF7,
       .bus = APB1,
       .irqn = USART3_IRQn,
#ifdef MODULE_PERIPH_DMA
       .dma_stream = DMA_STREAM_UNDEF,
       .dma_chan = UINT8_MAX
#endif
      },
      // CN12 P6(tx), P8(rx) and the Arduino CN7 connector, on D1(tx) and D0(rx) pins
      {.dev = USART6,
       .rcc_mask = RCC_APB2ENR_USART6EN,
       .rx_pin = GPIO_PIN(PORT_G, 9),
       .tx_pin = GPIO_PIN(PORT_G, 14),
       .rx_af = GPIO_AF8,
       .tx_af = GPIO_AF8,
       .bus = APB2,
       .irqn = USART6_IRQn,
#ifdef MODULE_PERIPH_DMA
       .dma_stream = DMA_STREAM_UNDEF,
       .dma_chan = UINT8_MAX
#endif
      }};

#define UART_0_ISR (isr_usart3)
#define UART_1_ISR (isr_usart6)

#define UART_NUMOF ARRAY_SIZE(uart_config)




#ifdef __cplusplus
}
#endif // __cplusplus
#endif // PERIPH_CONF_H