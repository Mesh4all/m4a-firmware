/*
 * Copyright (C) 2017 DAI Labor Technische Universität Berlin
 * Copyright (C) 2022 Mesh4all<contact@mesh4all.org>
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     boards_vs203
 * @{
 *
 * @file
 * @brief       Peripheral MCU configuration for the VS203 board
 *
 * @author      Anon Mall <anon.mall@gt-arc.com>
 * @author      luisan00 <luisan00@hotmail.com>
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "cpu.h"
#include "periph_cpu.h"
#include "periph/cpuid.h"

#include "cfg_clk_default.h"
#include "cfg_timer_default.h"

/**
 * @name    UART configuration
 * @{
 */

/**
 * @brief   List of available UART ports, pins are mapped as follows:
 *
 * | UART Dev | TX pin | RX pin | CTS pin| RTS pin |
 * |----------|--------|--------|--------|---------|
 * | 0        | PA0    | PA1    | Undef. | Undef.  |
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

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CONF_H */
/** @} */
