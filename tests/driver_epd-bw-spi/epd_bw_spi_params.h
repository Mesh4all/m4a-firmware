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
 * @author      luisan00 <luisan00@hotmail.com>
 */

#ifndef EPD_BW_SPI_PARAMS_H
#define EPD_BW_SPI_PARAMS_H

#ifdef __cplusplus
extern "C" {
#endif

#define EPD_BW_SPI_DISPLAY_X (200)
#define EPD_BW_SPI_DISPLAY_Y (200)

#define EPD_BW_SPI_CONTROLLER EPD_BW_SPI_CONTROLLER_IL3829

/**
 * @brief Board is Mesh4all-MB.
 *
 */
#ifdef BOARD_M4A_MB
#define EPD_BW_SPI_PARAM_SPI SPI_DEV(1)        /* select SPÎ device */
#define EPD_BW_SPI_PARAM_CS GPIO_PIN(PB, 15)   /* (IN) C. Select */
#define EPD_BW_SPI_PARAM_DC GPIO_PIN(PB, 14)   /* (IN) */
#define EPD_BW_SPI_PARAM_RST GPIO_PIN(PA, 21)  /* (IN) Reset*/
#define EPD_BW_SPI_PARAM_BUSY GPIO_PIN(PA, 20) /* (OUT) Device is Busy */
#endif

/**
 * @brief Board is STM32F469i-Discovery Kit.
 *
 */
#ifdef BOARD_STM32F469I_DISCO
#define EPD_BW_SPI_PARAM_SPI             /* select SPÎ device */
#define EPD_BW_SPI_PARAM_CS GPIO_UNDEF   /* (IN) C. Select */
#define EPD_BW_SPI_PARAM_DC GPIO_UNDEF   /* (IN) */
#define EPD_BW_SPI_PARAM_RST GPIO_UNDEF  /* (IN) Reset*/
#define EPD_BW_SPI_PARAM_BUSY GPIO_UNDEF /* (OUT) Device Busy */
#endif

#include_next "epd_bw_spi_params.h"

#ifdef __cplusplus
}
#endif

#endif /* EPD_BW_SPI_PARAMS_H */
