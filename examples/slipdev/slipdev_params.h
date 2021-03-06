/*
 * Copyright (C) 2015 Martine Lenders <mlenders@inf.fu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup slipdev
 * @{
 *
 * @file
 * @brief   slip parameters example, used by auto_init_gnrc_netif
 *
 * @author  Martine Lenders <mlenders@inf.fu-berlin.de>
 */

#ifndef SLIPDEV_PARAMS_H
#define SLIPDEV_PARAMS_H

#include "slipdev.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Struct of basic params of slip device.
 */

static slipdev_params_t slipdev_params[] = {
    {
        .uart = SLIP_UART,               /*!< Slip uart param */
        .baudrate = SLIP_BAUDRATE,        /*!< Slip uart baudrate param*/
    },
};

#ifdef __cplusplus
}
#endif
#endif /* SLIPDEV_PARAMS_H */
/** @} */
