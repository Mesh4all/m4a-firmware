/*
 * Copyright (C) 2017 DAI Labor Technische Universität Berlin
 * Copyright (C) 2022 Mesh4all<contact@mesh4all.org>
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     boards_meshme
 * @{
 *
 * @file        periph_conf.h
 * @brief       Peripheral MCU configuration for the Meshme board
 * @author      Anon Mall <anon.mall@gt-arc.com>
 * @author      Luis A. Ruiz <luisan00@hotmail.com>
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "cpu.h"
#include "periph_cpu.h"

#include "cfg_clk_default.h"
#include "cfg_timer_default.h"

/**
 * @name    [A]nalog to [D]igital [C]onverter configuration
 * @{
 */
/**
 * @brief   Selected [default] voltage reference is [SOC_ADC_ADCCON3_EREF_EXT],
 *          this can be changed according to the following table:
 *
 * | Register                   | Description                    |
 * |:---------------------------|:-------------------------------|
 * | SOC_ADC_ADCCON3_EREF_INT   | Internal reference             |
 * | SOC_ADC_ADCCON3_EREF_EXT   | External reference on AIN7 pin |
 * | SOC_ADC_ADCCON3_EREF_AVDD5 | AVDD5 pin                      |
 * | SOC_ADC_ADCCON3_EREF_DIFF  | External reference on AIN6-AIN7 differential input |
 *
 * @note default is [SOC_ADC_ADCCON3_EREF_EXT]
 * @see  periph_adc
 */
#define SOC_ADC_ADCCON3_EREF SOC_ADC_ADCCON3_EREF_AVDD5

/**
 * @brief   Selected default resolution is [ADC_RES_12BIT]
 *          but it can be changed according the next table:
 *
 * | Register      | Description                  |
 * |:--------------|:-----------------------------|
 * | ADC_RES_7BIT  | 7 bits per sample resolution |
 * | ADC_RES_9BIT  | 9 bits per sample    ""      |
 * | ADC_RES_10BIT | 10 bits per sample   ""      |
 * | ADC_RES_12BIT | 12 bits per sample   ""      |
 *
 * @note default is [ADC_RES_12BIT]
 */
#define SOC_ADC_RES ADC_RES_12BIT

/**
 * @brief Available ADC lines
 */
static const adc_conf_t adc_config[] = {
    GPIO_PIN(PORT_A, 6), /*!< GPIO_PA6 as ADC input */
    GPIO_PIN(PORT_A, 7), /*!< GPIO_PA7 as ADC input */
};

#define ADC_NUMOF ARRAY_SIZE(adc_config) /*!< Number of available adc lines */
/** @} */

/**
 * @name    [U]niversal [A]synchronous [R]eceiver [T]ransmitter peripheral.
 * @{
 */
/**
 * @brief   Two available UART ports but only one is set as follows:
 *
 * | UART Dev | TX pin | RX pin | CTS pin| RTS pin |
 * |:--------:|:------:|:------:|:------:|:-------:|
 * | 0        | PA0    | PA1    | Undef  | Undef   |
 * | ---      | ---    | ---    | ---    | ---     |
 *
 */
static const uart_conf_t uart_config[] = {
    {
        .dev = UART0_BASEADDR,
        .rx_pin = GPIO_PIN(PORT_A, 0),
        .tx_pin = GPIO_PIN(PORT_A, 1),
#ifdef MODULE_PERIPH_UART_HW_FC
        .cts_pin = GPIO_UNDEF,
        .rts_pin = GPIO_UNDEF,
#endif
    },
};

#define UART_0_ISR isr_uart0               /*!< interrupt function name mapping */
#define UART_NUMOF ARRAY_SIZE(uart_config) /*!< Number of UARTs */
/** @} */

/**
 * @name I2C Settings
 * @{
 */
#define I2C_IRQ_PRIO 1 /*!< i2c IRQ priority */

/**
 * @brief   There are 2 I2C ports available but only one is set as follows:
 *
 * | \#  | SCL | SDA | Speed |
 * |:---:|:---:|:---:|:-----:|
 * | 0   | PB0 | PB1 | Fast  |
 * | --- | --- | --- | ---   |
 *
 *  @note   I2C speed can be one of two modes available:
 * - Standard 100 kbps
 * - Fast 400 kbps
 */
static const i2c_conf_t i2c_config[] = {
    {
        .speed = I2C_SPEED_FAST,
        .scl_pin = GPIO_PIN(PORT_B, 0),
        .sda_pin = GPIO_PIN(PORT_B, 1),
    },
};
#define I2C_NUMOF ARRAY_SIZE(i2c_config) /*!< Available i2c ports */
/** @} */

/**
 * @name    SPI configuration
 * @{
 */

/**
 * @brief There are 2 SPI ports available but only one is set as follows:
 *
 * | Num | MOSI | MISO | SCK | CS  |
 * |:---:|:----:|:----:|:---:|:---:|
 * | 0   | PB4  | PB5  | PB6 | PB7 |
 * | --- | ---  | ---  | --- | --- |
 */
static const spi_conf_t spi_config[] = {
    {
        .num = 0,
        .mosi_pin = GPIO_PIN(PORT_B, 4),
        .miso_pin = GPIO_PIN(PORT_B, 5),
        .sck_pin = GPIO_PIN(PORT_B, 6),
        .cs_pin = GPIO_PIN(PORT_B, 7),
    },
};

#define SPI_NUMOF ARRAY_SIZE(spi_config) /*!< Available SPI ports */
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CONF_H */
/** @} */
