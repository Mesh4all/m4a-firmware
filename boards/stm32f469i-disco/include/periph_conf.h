/*
 * Copyright (c) 2021 Mesh4all <mesh4all.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @brief       Peripheral MCU configuration for the STM32F769I-DISCO board
 * @author      luisan00 <luisan00@hotmail.com>
 *
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

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
           // CN1 connector, Virtual Com Port
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
           // Alternate Function 8 (AF8)
           // On CN12 connector:        TX -> (PORT_C, 6),   RX -> (PORT_C, 7)
           // On Arduino CN7 connector: TX -> D1(PORT_G, 14),RX -> D0(PORT_G, 9)
           {.dev = USART6,
            .rcc_mask = RCC_APB2ENR_USART6EN,
            .rx_pin = GPIO_PIN(PORT_C, 7),
            .tx_pin = GPIO_PIN(PORT_C, 6),
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
#endif /* PERIPH_CONF_H */
