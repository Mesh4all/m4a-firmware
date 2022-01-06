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
 *@{
 * @file        periph_conf.h
 * @brief       Peripheral MCU configuration for **m4a_mb** board
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
 * @name Clock settings
 *
 * There are three choices for selection of CORECLOCK:
 *
 * - Usage of the Phase-Locked Loop (PLL) fed by the internal 8MHz oscillator divided by 8
 * - Usage of the 48 MHz DFLL fed by external oscillator running at 32 kHz
 * - Usage of the internal 8MHz oscillator directly, divided by N if needed
 *
 * The PLL option allows for the usage of a wider frequency range and a more
 * **stable clock with less jitter**. This is why this **option is default**.
 *
 * The target frequency is computed from the PLL multiplier and the PLL divisor.
 * Use the following formula to compute your values:
 *
 * **CORECLOCK = ((PLL_MUL + 1) * 1MHz) / PLL_DIV**
 *
 * @note The PLL circuit does not run with less than 32MHz while the maximum PLL
 *          frequency is 96MHz. So PLL_MULL must be between 31 and 95!
 *
 * The internal Oscillator used directly can lead to a slightly better power
 * efficiency to the cost of a less stable clock. Use this option when you know
 * what you are doing! The actual core frequency is adjusted as follows:
 *
 * **CORECLOCK = 8MHz / DIV**
 *
 * @note A core clock frequency below 1MHz is not recommended
 *
 * Default parameters are the following:
 *
 * - CLOCK_USE_PLL = True
 * - MUL = 47U
 * - DIV = 1
 *
 * @{
 */

/**
 * @brief Use Phase-Locked Loop [PLL] as reference clock: True
 */
#define CLOCK_USE_PLL (1)

/**
 * @brief Use External OScillator [XOSC32] as reference clock: False
 */
#define CLOCK_USE_XOSC32_DFLL (0)

/**
 * @brief OSCULP32K is factory calibrated to be around 32.768kHz but this values can
 * be of by a couple off % points, so prefer XOSC32K as default configuration.
 *
 * 0: use XOSC32K (always 32.768kHz) to clock GCLK2
 * 1: use OSCULP32K factory calibrated (~32.768kHz) to clock GCLK2
 */
#define GEN2_ULP32K (0)

#if CLOCK_USE_PLL
/**
 * @brief Set [MUL] to 47U if we are using [PLL]
 * @warning DO NOT EDIT; [MUL] should be between 31 and 95.
 */
#define CLOCK_PLL_MUL (47U)
/**
 * @brief Clock [DIV] 1U if we are using [PLL]
 * This don't need to be changed by now, but it can be adjusted for our requirements
 */
#define CLOCK_PLL_DIV (1U)
/**
 * @brief Definition for core clock if we are using [PLL] as reference.
 * @warning DO NOT EDIT; Generate the actual used core clock frequency.
 */
#define CLOCK_CORECLOCK (((CLOCK_PLL_MUL + 1) * 1000000U) / CLOCK_PLL_DIV)
#elif CLOCK_USE_XOSC32_DFLL
/**
 * @brief Core Clock set to 48 MHz if we are using XOSC32
 * @warning DO NOT EDIT; 48 MHz core clock
 */
#define CLOCK_CORECLOCK (48000000U)
/**
 * @brief External oscillator if we are using XOSC32 as reference.
 * @warning DO NOT EDIT; 32 kHz external oscillator
 */
#define CLOCK_XOSC32K (32768UL)
#define CLOCK_8MHZ (1)
#else
/**
 * @brief Clock [DIV] if we are using any other clock conf.
 * @warning DO NOT EDIT
 */
#define CLOCK_DIV (1U)
#define CLOCK_CORECLOCK (8000000 / CLOCK_DIV)
#endif
/** @} */

/**
 * @brief Timer peripheral configuration
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
    },
};

/**
 * @name Timers
 * @{
 */
#define TIMER_0_MAX_VALUE 0xffff                /*!< Max. Value */
#define TIMER_0_ISR isr_tc3                     /*!< Timer 0 ISR uses tc3 */
#define TIMER_1_ISR isr_tc4                     /*!< Timer 1 ISR uses tc4 */
#define TIMER_NUMOF ARRAY_SIZE(timer_config)    /*!< Number of timers available, default: 2 */
/** @} */

/**
 * @brief Settings for UART
 *
 * ## Pinout
 *
 * | UART | Function | GPIO pin  |
 * |:----:|----------|-----------|
 * | 0    | RX       | PA23      |
 * | ^    | TX       | PA22      |
 * | ^    | RTS      | Undefined |
 * | ^    | CTS      | Undefined |
 * | 1    | RX       | PA9       |
 * | ^    | TX       | PA8       |
 * | ^    | RTS      | Undefined |
 * | ^    | CTS      | Undefined |
 * | 2    | RX       | PA11      |
 * | ^    | TX       | PA10      |
 * | ^    | RTS      | Undefined |
 * | ^    | CTS      | Undefined |
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
    },
};

/**
 * @name UART configuration
 * @{
 */
#define UART_0_ISR isr_sercom3              /*!< UART 0 uses isr_sercom3 */
#define UART_1_ISR isr_sercom4              /*!< UART 1 uses isr_sercom4 */
#define UART_2_ISR isr_sercom5              /*!< UART 1 uses isr_sercom5 */
#define UART_NUMOF ARRAY_SIZE(uart_config)  /*!< Number of available UART ports */
/** @} */

/**
 * @name Enabled or Disabled PWM channels
 * @{
 */
#define PWM_0_EN 1              /*!< PWM 0 is enabled */
#define PWM_1_EN 0              /*!< PWM 1 is disabled */
#define PWM_2_EN 0              /*!< PWM 2 is disabled */
/** @} */

/**
 * @brief By default only channel 0 is enabled, PWM 1 and 2 can be enabled
 * by setting **PWM_1_EN** and/or **PWM_2_EN** to 1
 *
 * ### Pinout
 *
 * | channel | GPIO pin | TCC | Default stat. |
 * |:-------:|----------|:---:|---------------|
 * | 0       | PA12     | 0   | Enabled       |
 * | ^       | PA13     | 1   | Enabled       |
 * | 1       | PB12     | 0   | Disabled      |
 * | ^       | PB13     | 1   | Disabled      |
 * | 2       | PB02     | 0   | Disabled      |
 * | ^       | PB03     | 1   | Disabled      |
 */
#if PWM_0_EN
/**
 * @brief Settings for PWM channel 1 if it's enabled
 */
static const pwm_conf_chan_t pwm_chan0_config[] = {
    {GPIO_PIN(PA, 12), GPIO_MUX_E, 0},
    {GPIO_PIN(PA, 13), GPIO_MUX_E, 1},
};
#endif
#if PWM_1_EN
/**
 * @brief Settings for PWM channel 1 if it's enabled
 */
static const pwm_conf_chan_t pwm_chan1_config[] = {
    {GPIO_PIN(PB, 12), GPIO_MUX_E, 0},
    {GPIO_PIN(PB, 13), GPIO_MUX_E, 1},
};
#endif
#if PWM_2_EN
/**
 * @brief Settings for PWM channel 2 if it's enabled
 */
static const pwm_conf_chan_t pwm_chan2_config[] = {
    {GPIO_PIN(PB, 02), GPIO_MUX_E, 0},
    {GPIO_PIN(PB, 03), GPIO_MUX_E, 1},
};
#endif

/**
 * @brief Settings for PWM peripheral
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
 * @name PWM config
 * @{
 */
#define PWM_NUMOF ARRAY_SIZE(pwm_config)        /*!< Number of PWM channels, default: only chan0 */
/** @} */

/**
 * @brief SPI configuration
 *
 * ### SPI Pinout
 *
 * | SPI | name | pin  |
 * |:---:|------|------|
 * | 0   | miso | PA4  |
 * | ^   | mosi | PA6  |
 * | ^   | clk  | PA7  |
 * | 1   | miso | PA16 |
 * | ^   | mosi | PA18 |
 * | ^   | clk  | PA19 |
 * | 2   | miso | PB16 |
 * | ^   | mosi | PB22 |
 * | ^   | clk  | PB23 |
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
    },
};

/**
 * @name Number of SPI
 * @{
 */
#define SPI_NUMOF ARRAY_SIZE(spi_config)                /*!< 3 SPI are available */
/** @} */

/**
 * @brief I2C configuration
 *
 * ### Pinout
 *
 * | device | name | pin |
 * |--------|------|-----|
 * | I2C_0  | SCL  | PA9 |
 * | I2C_0  | SDA  | PA8 |
 */
static const i2c_conf_t i2c_config[] = {
    {
        .dev = &(SERCOM2->I2CM),
        .speed = I2C_SPEED_NORMAL,
        .scl_pin = GPIO_PIN(PA, 9),
        .sda_pin = GPIO_PIN(PA, 8),
        .mux = GPIO_MUX_D,
        .gclk_src = SAM0_GCLK_MAIN,
        .flags = I2C_FLAG_NONE,
    },
};

/**
 * @name Number of I2C
 * @{
 */
#define I2C_NUMOF ARRAY_SIZE(i2c_config)                /*!< Number of I2C available */
/** @} */

/**
 * @name RTT configuration
 * @{
 */
#ifndef RTT_FREQUENCY
#define RTT_FREQUENCY (32768U)                          /*!< RTT is set to 32768 */
#endif
/** @} */

/**
 * @name ADC Configuration
 * @{
 */
#define ADC_PRESCALER ADC_CTRLB_PRESCALER_DIV512        /*!< Prescaler: DIV 512 */
#define ADC_NEG_INPUT ADC_INPUTCTRL_MUXNEG_GND          /*!< Neg. Input: GND */
#define ADC_GAIN_FACTOR_DEFAULT ADC_INPUTCTRL_GAIN_1X   /*!< Gain Factor: 1x */
#define ADC_REF_DEFAULT ADC_REFCTRL_REFSEL_INT1V        /*!< Reference: Internal 1.0 Volt */
/** @} */

/**
 * @brief ADC channels
 *
 * ## [A]nalog-to-[D]igital [C]onverters
 *
 * ### Pinout
 *
 * | Channel | GPIO pin |
 * |:-------:|----------|
 * | 0       | PB0      |
 * | 1       | PB1      |
 * | 2       | PA10     |
 * | 3       | PA11     |
 * | 4       | PA2      |
 * | 5       | PA3      |
 */
static const adc_conf_chan_t adc_channels[] = {
    {GPIO_PIN(PB, 0), ADC_INPUTCTRL_MUXPOS_PIN8},
    {GPIO_PIN(PB, 1), ADC_INPUTCTRL_MUXPOS_PIN9},
    {GPIO_PIN(PA, 10), ADC_INPUTCTRL_MUXPOS_PIN18},
    {GPIO_PIN(PA, 11), ADC_INPUTCTRL_MUXPOS_PIN19},
    {GPIO_PIN(PA, 2), ADC_INPUTCTRL_MUXPOS_PIN0},
    {GPIO_PIN(PA, 3), ADC_INPUTCTRL_MUXPOS_PIN1},
};

/**
 * @name Number of ADC channels
 * @{
 */
#define ADC_NUMOF ARRAY_SIZE(adc_channels)              /*!< Number of ADC channels is 6*/
/** @} */

/**
 * @name DAC Settings
 * @{
 */
#define DAC_CLOCK SAM0_GCLK_1MHZ                        /*!< Uses 1 MHz clock*/
#define DAC_VREF DAC_CTRLB_REFSEL_AVCC                  /*!< Select VCC as reference */
/** @} */

/**
 * @brief USB peripheral settings
 *
 * ### Pinout
 *
 * | function | pin  |
 * |----------|------|
 * | D-       | PA24 |
 * | D+       | PA25 |
 */
static const sam0_common_usb_config_t sam_usbdev_config[] = {
    {
        .dm = GPIO_PIN(PA, 24),
        .dp = GPIO_PIN(PA, 25),
        .d_mux = GPIO_MUX_G,
        .device = &USB->DEVICE,
        .gclk_src = SAM0_GCLK_MAIN,
    },
};

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CONF_H */
/** @} */
