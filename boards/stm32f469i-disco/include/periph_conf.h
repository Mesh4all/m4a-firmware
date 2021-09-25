/* 
 * Copyright (c) 2021 Mesh4all <mesh4all.org>
 
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
        * @brief    UART configuration
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

    /**
     * @brief   I2C configuration
     * 
     */
    static const i2c_conf_t i2c_config[] = {
        // i2c1 is available on CN11: SDA(PORT_B, 9), SCL(PORT_B, 8)
        {
            .dev = I2C1,
            .speed = I2C_SPEED_NORMAL,
            .scl_pin = GPIO_PIN(PORT_B, 8),
            .sda_pin = GPIO_PIN(PORT_B, 9),
            .scl_af = GPIO_AF4,
            .sda_af = GPIO_AF4,
            .bus = APB1,
            .rcc_mask = RCC_APB1ENR_I2C1EN,
            .clk = CLOCK_APB1,
            .irqn = I2C1_EV_IRQn,
        },
        // i2c2 controls the audio DAC-SAI(addr: 0x94), Max. speed 100 KHz
        {
            .dev = I2C2,
            .speed = I2C_SPEED_NORMAL,
            .scl_pin = GPIO_PIN(PORT_H, 4),
            .sda_pin = GPIO_PIN(PORT_H, 5),
            .scl_af = GPIO_AF4,
            .sda_af = GPIO_AF4,
            .bus = APB1,
            .rcc_mask = RCC_APB1ENR_I2C2EN,
            .clk = CLOCK_APB1,
            .irqn = I2C2_EV_IRQn,
        }};

#define I2C_0_ISR isr_i2c1_ev
#define I2C_1_ISR isr_i2c2_ev

#define I2C_NUMOF ARRAY_SIZE(i2c_config)

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // PERIPH_CONF_H