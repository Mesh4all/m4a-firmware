/*
 * Copyright (C) 2021 Mesh For All <mesh4all.org>
 *
 * This file is is licensed under the Apache License, Version 2.0,
 * you may not use this file except in compliance with the License.
 * See the LICENSE file in the root folder for more information
 */

/**
 * @ingroup         boards_m4a-mb
 * 
 * @brief           Peripheral MCU configuration for m4a_mb
 * 
 * @author          luisan00 <luisan00@hotmail.com>
 * 
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

#include <stdint.h>
#include "cpu.h"
#include "periph_cpu.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief   External oscillator and clock configuration
 *
 */
#define CLOCK_USE_PLL (1)
#define CLOCK_USE_XOSC32_DFLL (0)

#define GEN2_ULP32K (0)

#if CLOCK_USE_PLL
/* DO NOT EDIT; [PLL] output frequency */
/* [MUL] should be between 31 and 95, both numbers included */
#define CLOCK_PLL_MUL (47U)
/* [DIV] don't need to be changed by now but it can be adjusted */
/* for our requirements */
#define CLOCK_PLL_DIV (1U)
/* DO NOT EDIT; Generate the actual used core clock frequency */
#define CLOCK_CORECLOCK (((CLOCK_PLL_MUL + 1) * 1000000U) / CLOCK_PLL_DIV)
#elif CLOCK_USE_XOSC32_DFLL
/* DO NOT EDIT; 48 MHz core clock */
#define CLOCK_CORECLOCK (48000000U)
/* DO NOT EDIT; 32 kHz ext. oscillator */
#define CLOCK_XOSC32K (32768UL)
#define CLOCK_8MHZ (1)
#else
/* DO NOT EDIT */
#define CLOCK_DIV (1U)
#define CLOCK_CORECLOCK (8000000 / CLOCK_DIV)
#endif

    /**
     * @brief   Timer peripheral configuration
     *
     */
    static const tc32_conf_t timer_config[] = {
        {
            .dev = TC3,
            .irq = TC3_IRQn,
            .pm_mask = PM_APBCMASK_TC3,
            .gclk_ctrl = GCLK_CLKCTRL_ID_TCC2_TC3,
#if CLOCK_USE_PLL || CLOCK_USE_XOSC32_DFLL
            .gclk_src = SAM0_GCLK_1MHZ,
#else
        .gclk_src = SAM0_GCLK_MAIN,
#endif
            .flags = TC_CTRLA_MODE_COUNT16,
        },
        {
            .dev = TC4,
            .irq = TC4_IRQn,
            .pm_mask = PM_APBCMASK_TC4 | PM_APBCMASK_TC5,
            .gclk_ctrl = GCLK_CLKCTRL_ID_TC4_TC5,
#if CLOCK_USE_PLL || CLOCK_USE_XOSC32_DFLL
            .gclk_src = SAM0_GCLK_1MHZ,
#else
        .gclk_src = SAM0_GCLK_MAIN,
#endif
            .flags = TC_CTRLA_MODE_COUNT32,
        }};

#define TIMER_0_MAX_VALUE 0xffff
#define TIMER_0_ISR isr_tc3
#define TIMER_1_ISR isr_tc4

/**
 * @brief 
 * 
 */
#define TIMER_NUMOF ARRAY_SIZE(timer_config)

    /**
     * @brief UART configuration
     * 
     */
    static const uart_conf_t uart_config[] = {
        {
            .dev = &SERCOM3->USART,
            .rx_pin = GPIO_PIN(PA, 23),
            .tx_pin = GPIO_PIN(PA, 22),
#ifdef MODULE_PERIPH_UART_HW_FC
            .rts_pin = GPIO_UNDEF,
            .cts_pin = GPIO_UNDEF,
#endif
            .mux = GPIO_MUX_C,
            .rx_pad = UART_PAD_RX_1,
            .tx_pad = UART_PAD_TX_0,
            .flags = UART_FLAG_NONE,
            .gclk_src = SAM0_GCLK_MAIN,
        },
        {
            .dev = &SERCOM4->USART,
            .rx_pin = GPIO_PIN(PB, 9),
            .tx_pin = GPIO_PIN(PB, 8),
#ifdef MODULE_PERIPH_UART_HW_FC
            .rts_pin = GPIO_UNDEF,
            .cts_pin = GPIO_UNDEF,
#endif
            .mux = GPIO_MUX_D,
            .rx_pad = UART_PAD_RX_1,
            .tx_pad = UART_PAD_TX_0,
            .flags = UART_FLAG_NONE,
            .gclk_src = SAM0_GCLK_MAIN,
        },
        {
            .dev = &SERCOM4->USART,
            .rx_pin = GPIO_PIN(PB, 11),
            .tx_pin = GPIO_PIN(PB, 10),
#ifdef MODULE_PERIPH_UART_HW_FC
            .rts_pin = GPIO_UNDEF,
            .cts_pin = GPIO_UNDEF,
#endif
            .mux = GPIO_MUX_D,
            .rx_pad = UART_PAD_RX_3,
            .tx_pad = UART_PAD_TX_2,
            .flags = UART_FLAG_NONE,
            .gclk_src = SAM0_GCLK_MAIN,
        }};

#define UART_0_ISR isr_sercom3
#define UART_1_ISR isr_sercom4
#define UART_2_ISR isr_sercom5

/**
 * @brief Number of available UART ports
 * 
 */
#define UART_NUMOF ARRAY_SIZE(uart_config)

/**
 * @brief   PWM configuration. 
 *          By default the first one is active (1) 
 *          and the other two, can be assigned to 0 (Disabled).
 * 
 */
#define PWM_0_EN 1
#define PWM_1_EN 0
#define PWM_2_EN 0

/**
 * @brief   PWM0 configuration if it's enabled
 *          Default 1 enabled
 * 
 */
#if PWM_0_EN
    static const pwm_conf_chan_t pwm_chan0_config[] = {
        {GPIO_PIN(PA, 12), GPIO_MUX_E, 0},
        {GPIO_PIN(PA, 13), GPIO_MUX_E, 1},
    };
#endif

/**
 * @brief   PWM 1 configuration if it's enabled
 *          Default 0 disabled
 * 
 */
#if PWM_1_EN
    static const pwm_conf_chan_t pwm_chan1_config[] = {
        {GPIO_PIN(PB, 12), GPIO_MUX_E, 0},
        {GPIO_PIN(PB, 13), GPIO_MUX_E, 1},
    };
#endif

/**
 * @brief   PWM2 configuration if it's enabled
 *          Default 0 disabled
 * 
 */
#if PWM_2_EN
    static const pwm_conf_chan_t pwm_chan2_config[] = {
        /* GPIO pin, MUX value, TCC channel */
        {GPIO_PIN(PB, 02), GPIO_MUX_E, 0},
        {GPIO_PIN(PB, 03), GPIO_MUX_E, 1},
    };
#endif

    /**
     * @brief PWM device configuration
     * 
     */
    static const pwm_conf_t pwm_config[] = {
#if PWM_0_EN
        {TCC_CONFIG(TCC2), pwm_chan0_config, ARRAY_SIZE(pwm_chan0_config), SAM0_GCLK_MAIN},
#endif
#if PWM_1_EN
        {TCC_CONFIG(TC4), pwm_chan1_config, ARRAY_SIZE(pwm_chan1_config), SAM0_GCLK_MAIN},
#endif
#if PWM_2_EN
        {TCC_CONFIG(TC6), pwm_chan2_config, ARRAY_SIZE(pwm_chan2_config), SAM0_GCLK_MAIN},
#endif
    };

/**
 * @brief Number of PWM devices available
 * 
 */
#define PWM_NUMOF ARRAY_SIZE(pwm_config)

    /**
     * @brief SPI configuration
     * 
     */
    static const spi_conf_t spi_config[] = {
        {
            .dev = &SERCOM0->SPI,
            .miso_pin = GPIO_PIN(PA, 4),
            .mosi_pin = GPIO_PIN(PA, 6),
            .clk_pin = GPIO_PIN(PA, 7),
            .miso_mux = GPIO_MUX_D,
            .mosi_mux = GPIO_MUX_D,
            .clk_mux = GPIO_MUX_D,
            .miso_pad = SPI_PAD_MISO_0,
            .mosi_pad = SPI_PAD_MOSI_2_SCK_3,
            .gclk_src = SAM0_GCLK_MAIN,
#ifdef MODULE_PERIPH_DMA
            .tx_trigger = SERCOM0_DMAC_ID_TX,
            .rx_trigger = SERCOM0_DMAC_ID_RX,
#endif
        },
        {
            .dev = &SERCOM1->SPI,
            .miso_pin = GPIO_PIN(PA, 16),
            .mosi_pin = GPIO_PIN(PA, 18),
            .clk_pin = GPIO_PIN(PA, 19),
            .miso_mux = GPIO_MUX_C,
            .mosi_mux = GPIO_MUX_C,
            .clk_mux = GPIO_MUX_C,
            .miso_pad = SPI_PAD_MISO_0,
            .mosi_pad = SPI_PAD_MOSI_2_SCK_3,
            .gclk_src = SAM0_GCLK_MAIN,
#ifdef MODULE_PERIPH_DMA
            .tx_trigger = SERCOM1_DMAC_ID_TX,
            .rx_trigger = SERCOM1_DMAC_ID_RX,
#endif
        },
        {
            .dev = &SERCOM5->SPI,
            .miso_pin = GPIO_PIN(PB, 16),
            .mosi_pin = GPIO_PIN(PB, 22),
            .clk_pin = GPIO_PIN(PB, 23),
            .miso_mux = GPIO_MUX_C,
            .mosi_mux = GPIO_MUX_D,
            .clk_mux = GPIO_MUX_D,
            .miso_pad = SPI_PAD_MISO_0,
            .mosi_pad = SPI_PAD_MOSI_2_SCK_3,
            .gclk_src = SAM0_GCLK_MAIN,
#ifdef MODULE_PERIPH_DMA
            .tx_trigger = SERCOM5_DMAC_ID_TX,
            .rx_trigger = SERCOM5_DMAC_ID_RX,
#endif
        }};

#define SPI_NUMOF ARRAY_SIZE(spi_config)

    /**
     * @brief I2C configuration
     *
     */
    static const i2c_conf_t i2c_config[] = {
        {.dev = &(SERCOM2->I2CM),
         .speed = I2C_SPEED_NORMAL,
         .scl_pin = GPIO_PIN(PA, 9),
         .sda_pin = GPIO_PIN(PA, 8),
         .mux = GPIO_MUX_D,
         .gclk_src = SAM0_GCLK_MAIN,
         .flags = I2C_FLAG_NONE}};

/**
 * @brief Number of I2C devices availble
 * 
 */
#define I2C_NUMOF ARRAY_SIZE(i2c_config)

/**
 * @brief RTT configuration
 *
 */
#ifndef RTT_FREQUENCY
#define RTT_FREQUENCY (32768U)
#endif

/**
 * @brief ADC Configuration
 *
 */
#define ADC_PRESCALER ADC_CTRLB_PRESCALER_DIV512
#define ADC_NEG_INPUT ADC_INPUTCTRL_MUXNEG_GND
#define ADC_GAIN_FACTOR_DEFAULT ADC_INPUTCTRL_GAIN_1X
#define ADC_REF_DEFAULT ADC_REFCTRL_REFSEL_INT1V

    static const adc_conf_chan_t adc_channels[] = {
        {GPIO_PIN(PB, 0), ADC_INPUTCTRL_MUXPOS_PIN8},
        {GPIO_PIN(PB, 1), ADC_INPUTCTRL_MUXPOS_PIN9},
        {GPIO_PIN(PA, 10), ADC_INPUTCTRL_MUXPOS_PIN18},
        {GPIO_PIN(PA, 11), ADC_INPUTCTRL_MUXPOS_PIN19},
        {GPIO_PIN(PA, 2), ADC_INPUTCTRL_MUXPOS_PIN0},
        {GPIO_PIN(PA, 3), ADC_INPUTCTRL_MUXPOS_PIN1}};

/**
 * @brief Number of ADC channels available 
 * 
 */
#define ADC_NUMOF ARRAY_SIZE(adc_channels)

/**
 * @brief DAC configuration
 *
 */
#define DAC_CLOCK SAM0_GCLK_1MHZ

/**
 * @brief Use VCC as reference voltage for the DAC
 * 
 */
#define DAC_VREF DAC_CTRLB_REFSEL_AVCC

    /**
     * @brief USB peripheral configuration
     * 
     */
    static const sam0_common_usb_config_t sam_usbdev_config[] = {
        {
            .dm = GPIO_PIN(PA, 24),
            .dp = GPIO_PIN(PA, 25),
            .d_mux = GPIO_MUX_G,
            .device = &USB->DEVICE,
            .gclk_src = SAM0_GCLK_MAIN,
        }};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // PERIPH_CONF_H