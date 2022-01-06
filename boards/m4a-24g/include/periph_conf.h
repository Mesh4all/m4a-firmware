/*
 * Copyright (C) 2014-2015 Freie Universität Berlin
 * Copyright (C) 2021 Mesh4all <contact@mesh4all.org>
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
 * @brief       Configuration of CPU peripherals for the m4a-24g board
 *
 * @author      Thomas Eichinger <thomas.eichinger@fu-berlin.de>
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 * @author      Peter Kietzmann <peter.kietzmann@haw-hamburg.de>
 * @author      Luis A. Ruiz <luisan00@hotmail.com>
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

#include <stdint.h>

#include "cpu.h"
#include "periph_cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name   External oscillator and clock configuration
 *
 * There are three choices for selection of CORECLOCK:
 *
 * - usage of the Phase-Locked Loop[PLL] fed by the internal 8MHz oscillator divided by 8
 * - usage of the 48 MHz DFLL fed by external oscillator running at 32 kHz
 * - usage of the internal 8MHz oscillator directly, divided by N if needed
 *
 *
 * The PLL option allows for the usage of a wider frequency range and a more
 * stable clock with less jitter. This is why this option is default.
 *
 * The target frequency is computed from the PLL multiplier and the PLL divisor.
 * Use the following formula to compute your values:
 *
 * **CORECLOCK = ((PLL_MUL + 1) * 1MHz) / PLL_DIV**
 *
 * @note: The PLL circuit does not run with less than 32MHz while the maximum PLL
 *       frequency is 96MHz. So PLL_MULL must be between 31 and 95!
 *
 *
 * The internal Oscillator used directly can lead to a slightly better power
 * efficiency to the cost of a less stable clock. Use this option when you know
 * what you are doing! The actual core frequency is adjusted as follows:
 *
 * **CORECLOCK = 8MHz / DIV**
 *
 * @note: A core clock frequency below 1MHz is not recommended
 *
 * Default parameters are the following:
 *
 * - CLOCK_USE_PLL = True
 * - MUL = 47U
 * - DIV = 1U
 *
 * @{
 */
/**
 * @brief Use Phase-Locked Loop [PLL] as reference clock: True
 */
#define CLOCK_USE_PLL               (1)

/**
 * @brief Use External OScillator [XOSC32] as reference clock: False
 */
#define CLOCK_USE_XOSC32_DFLL       (0)
/**
 * @brief OSCULP32K is factory calibrated to be around 32.768kHz but this values can
 * be of by a couple off % points, so prefer XOSC32K as default configuration.
 *
 * 0: use XOSC32K (always 32.768kHz) to clock GCLK2
 * 1: use OSCULP32K factory calibrated (~32.768kHz) to clock GCLK2
 */
#define GEN2_ULP32K                 (0)

#if CLOCK_USE_PLL
/**
 * @brief Set [MUL] to 47U if we are using [PLL]
 * @warning DO NOT EDIT; [MUL] should be between 31 and 95.
 */
#define CLOCK_PLL_MUL       (47U)
/**
 * @brief Clock [DIV] 1U if we are using [PLL]
 * This don't need to be changed by now, but it can be adjusted for our requirements
 */
#define CLOCK_PLL_DIV       (1U)
/**
 * @brief Definition for core clock if we are using [PLL] as reference.
 * @warning DO NOT EDIT; Generate the actual used core clock frequency.
 */
#define CLOCK_CORECLOCK     (((CLOCK_PLL_MUL + 1) * 1000000U) / CLOCK_PLL_DIV)
#elif CLOCK_USE_XOSC32_DFLL
/**
 * @brief Core Clock set to 48 MHz if we are using XOSC32
 * @warning DO NOT EDIT; 48 MHz core clock
 */
#define CLOCK_CORECLOCK     (48000000U)
/**
 * @brief External oscillator if we are using XOSC32 as reference.
 * @warning DO NOT EDIT; 32 kHz external oscillator
 */
#define CLOCK_XOSC32K       (32768UL)
#define CLOCK_8MHZ          (1)
#else
/**
 * @brief Clock [DIV] if we are using any other clock conf.
 * @warning DO NOT EDIT
 */
#define CLOCK_DIV           (1U)
#define CLOCK_CORECLOCK     (8000000 / CLOCK_DIV)
#endif
/** @} */

/**
 * @brief    Timer peripheral configuration
 */
static const tc32_conf_t timer_config[] = {
    {
        .dev            = TC3,
        .irq            = TC3_IRQn,
        .pm_mask        = PM_APBCMASK_TC3,
        .gclk_ctrl      = GCLK_CLKCTRL_ID_TCC2_TC3,
#if CLOCK_USE_PLL || CLOCK_USE_XOSC32_DFLL
        .gclk_src       = SAM0_GCLK_1MHZ,
#else
        .gclk_src       = SAM0_GCLK_MAIN,
#endif
        .flags          = TC_CTRLA_MODE_COUNT16,
    },
    {
        .dev            = TC4,
        .irq            = TC4_IRQn,
        .pm_mask        = PM_APBCMASK_TC4 | PM_APBCMASK_TC5,
        .gclk_ctrl      = GCLK_CLKCTRL_ID_TC4_TC5,
#if CLOCK_USE_PLL || CLOCK_USE_XOSC32_DFLL
        .gclk_src       = SAM0_GCLK_1MHZ,
#else
        .gclk_src       = SAM0_GCLK_MAIN,
#endif
        .flags          = TC_CTRLA_MODE_COUNT32,
    },
};

/**
 * @name name defined settings for TIMERs
 * @{
 */
#define TIMER_0_MAX_VALUE   0xffff                      /*!< Max. value for TIMER 0 */
#define TIMER_0_ISR         isr_tc3                     /*!< Internal function name */
#define TIMER_1_ISR         isr_tc4                     /*!< Internal function name */
#define TIMER_NUMOF         ARRAY_SIZE(timer_config)    /*!< Number of TIMER peripherals, 2 */
/** @} */

/**
 * @brief UART config
 *
 */
static const uart_conf_t uart_config[] = {
    {
        .dev      = &SERCOM0->USART,
        .rx_pin   = GPIO_PIN(PA, 5),
        .tx_pin   = GPIO_PIN(PA, 4),
#ifdef MODULE_PERIPH_UART_HW_FC
        .rts_pin  = GPIO_PIN(PA, 6),
        .cts_pin  = GPIO_PIN(PA, 7),
#endif
        .mux      = GPIO_MUX_D,
        .rx_pad   = UART_PAD_RX_1,
#ifdef MODULE_PERIPH_UART_HW_FC
        .tx_pad   = UART_PAD_TX_0_RTS_2_CTS_3,
#else
        .tx_pad   = UART_PAD_TX_0,
#endif
        .flags    = UART_FLAG_NONE,
        .gclk_src = SAM0_GCLK_MAIN,
    },
    {
        .dev      = &SERCOM5->USART,
        .rx_pin   = GPIO_PIN(PA, 23),
        .tx_pin   = GPIO_PIN(PA, 22),
#ifdef MODULE_PERIPH_UART_HW_FC
        .rts_pin  = GPIO_PIN(PB, 22),
        .cts_pin  = GPIO_PIN(PB, 23),
#endif
        .mux      = GPIO_MUX_D,
        .rx_pad   = UART_PAD_RX_1,
#ifdef MODULE_PERIPH_UART_HW_FC
        .tx_pad   = UART_PAD_TX_0_RTS_2_CTS_3,
#else
        .tx_pad   = UART_PAD_TX_0,
#endif
        .flags    = UART_FLAG_NONE,
        .gclk_src = SAM0_GCLK_MAIN,
    }
};

/**
 * @name interrupt function name mapping and number of UART peripherals defined
 * @{
 */
#define UART_0_ISR          isr_sercom0             /*!< Internal function name */
#define UART_1_ISR          isr_sercom5             /*!< Internal function name */
#define UART_NUMOF          ARRAY_SIZE(uart_config) /*!< Number of UARTs available */
/** @} */

/**
 * @name    PWM configuration
 * @{
 */
#define PWM_0_EN            1   /*!< PWM_0 is enabled by default */
#define PWM_1_EN            1   /*!< PWM_1 is enabled by default */
/** @} */

#if PWM_0_EN
/**
 * @brief IF PWM_0 is enabled then the following values are configured:
 * @note Default value for **PWM_0_EN** is 1, **ENABLED**
 *
 * | PWM  | GPIO pin   | MUX value  | Channel |
 * |:----:|------------|------------|:-------:|
 * | 0    | PA6        | GPIO_MUX_E | 0       |
 * | 1    | PA7        | GPIO_MUX_E | 1       |
 *
 */
static const pwm_conf_chan_t pwm_chan0_config[] = {
    { GPIO_PIN(PA, 6), GPIO_MUX_E, 0 },
    { GPIO_PIN(PA, 7), GPIO_MUX_E, 1 },
};
#endif

#if PWM_1_EN
/**
 * @brief IF PWM_1 is enabled then the following values are configured:
 * @note Default value for **PWM_1_EN** is 1, **ENABLED**
 *
 * | PWM  | GPIO pin   | MUX value  | Channel |
 * |:----:|------------|------------|:-------:|
 * | 0    | PA16       | GPIO_MUX_F | 0       |
 * | 1    | PA18       | GPIO_MUX_F | 2       |
 * | 2    | PA19       | GPIO_MUX_F | 3       |
 *
 */
static const pwm_conf_chan_t pwm_chan1_config[] = {
    { GPIO_PIN(PA, 16), GPIO_MUX_F, 0 },
    { GPIO_PIN(PA, 18), GPIO_MUX_F, 2 },
    { GPIO_PIN(PA, 19), GPIO_MUX_F, 3 },
};
#endif

/**
 * @brief PWM device configuration
 */
static const pwm_conf_t pwm_config[] = {
#if PWM_0_EN
    {TCC_CONFIG(TCC1), pwm_chan0_config, ARRAY_SIZE(pwm_chan0_config), SAM0_GCLK_MAIN},
#endif
#if PWM_1_EN
    {TCC_CONFIG(TCC0), pwm_chan1_config, ARRAY_SIZE(pwm_chan1_config), SAM0_GCLK_MAIN},
#endif
};

/**
 * @name Number of PWM devices defined
 * @{
 */
#define PWM_NUMOF           ARRAY_SIZE(pwm_config)  /*!< Number of SPI peripheral available */
/** @} */

/**
 * @brief SPI config
 */
static const spi_conf_t spi_config[] = {
    {
        .dev      = &SERCOM4->SPI,
        .miso_pin = GPIO_PIN(PC, 19),
        .mosi_pin = GPIO_PIN(PB, 30),
        .clk_pin  = GPIO_PIN(PC, 18),
        .miso_mux = GPIO_MUX_F,
        .mosi_mux = GPIO_MUX_F,
        .clk_mux  = GPIO_MUX_F,
        .miso_pad = SPI_PAD_MISO_0,
        .mosi_pad = SPI_PAD_MOSI_2_SCK_3,
        .gclk_src = SAM0_GCLK_MAIN,
#ifdef MODULE_PERIPH_DMA
        .tx_trigger = SERCOM4_DMAC_ID_TX,
        .rx_trigger = SERCOM4_DMAC_ID_RX,
#endif
    },
    {
        .dev      = &SERCOM5->SPI,
        .miso_pin = GPIO_PIN(PB, 2),
        .mosi_pin = GPIO_PIN(PB, 22),
        .clk_pin  = GPIO_PIN(PB, 23),
        .miso_mux = GPIO_MUX_D,
        .mosi_mux = GPIO_MUX_D,
        .clk_mux  = GPIO_MUX_D,
        .miso_pad = SPI_PAD_MISO_0,
        .mosi_pad = SPI_PAD_MOSI_2_SCK_3,
        .gclk_src = SAM0_GCLK_MAIN,
#ifdef MODULE_PERIPH_DMA
        .tx_trigger = SERCOM5_DMAC_ID_TX,
        .rx_trigger = SERCOM5_DMAC_ID_RX,
#endif
    },
};

/**
 * @name    Number of SPI devices defined
 * @{
 */
#define SPI_NUMOF           ARRAY_SIZE(spi_config)  /*!< SPI available */
/** @} */

/**
 * @brief I2C devices configuration
 *
 * | I2C | SCL pin | SDA pin |
 * |:---:|:--------|:--------|
 * | 0   | PA17    | PA16    |
 */
static const i2c_conf_t i2c_config[] = {
    {
        .dev      = &(SERCOM3->I2CM),
        .speed    = I2C_SPEED_NORMAL,
        .scl_pin  = GPIO_PIN(PA, 17),
        .sda_pin  = GPIO_PIN(PA, 16),
        .mux      = GPIO_MUX_D,
        .gclk_src = SAM0_GCLK_MAIN,
        .flags    = I2C_FLAG_NONE
     }
};

/**
 * @name    Number of I2C devices defined, default is 1
 * @{
 */
#define I2C_NUMOF          ARRAY_SIZE(i2c_config)   /*!< 1 I2C peripheral is availble */
/** @} */

/**
 * @name RTT configuration
 * @{
 */
#ifndef RTT_FREQUENCY
/**
 * @brief   The desired frequency for the Real Time Timer peripheral in Hz.
 * @see  For changes checkout rtt.c and rtt.h
 *
 */
#define RTT_FREQUENCY       (32768U)
#endif
#define RTT_MIN_OFFSET      (10U)       /*!< ?? */
/** @} */

/**
 * @name Default ADC values
 * @{
 */
#define ADC_PRESCALER   ADC_CTRLB_PRESCALER_DIV512      /*!< Prescaler: 512 */
#define ADC_NEG_INPUT   ADC_INPUTCTRL_MUXNEG_GND        /*!< Neg. Input: GND */
#define ADC_GAIN_FACTOR_DEFAULT ADC_INPUTCTRL_GAIN_1X   /*!< Gain Factor: 1x */
#define ADC_REF_DEFAULT ADC_REFCTRL_REFSEL_INT1V        /*!< Reference: Internal 1.0 Volt */
/** @} */

/**
 * @brief ADC configuration
 */
static const adc_conf_chan_t adc_channels[] = {
    {GPIO_PIN(PA, 6), ADC_INPUTCTRL_MUXPOS_PIN6},
    {GPIO_PIN(PA, 7), ADC_INPUTCTRL_MUXPOS_PIN7},
};

/**
 * @name Number of ADCs available
 * @{
 */
#define ADC_NUMOF   ARRAY_SIZE(adc_channels)    /*!< Number of ADCs available */
/** @} */

/**
 * @brief USB peripheral configuration
 */
static const sam0_common_usb_config_t sam_usbdev_config[] = {
    {
        .dm     = GPIO_PIN(PA, 24),
        .dp     = GPIO_PIN(PA, 25),
        .d_mux  = GPIO_MUX_G,
        .device = &USB->DEVICE,
        .gclk_src = SAM0_GCLK_MAIN,
    }
};

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CONF_H */
/** @} */
