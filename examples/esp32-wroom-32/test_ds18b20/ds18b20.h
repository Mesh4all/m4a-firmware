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
 * @ingroup     ds18b20
 *
 * @file
 * @{
 * @brief       This is the required params to use the ds18b20 sensor
 *
 * @author      Eduardo Azócar <eduardo@turpialdev.com>
 *
 */

#ifndef DS18B20_H
#define DS18B20_H

#include "ds18.h"
#ifdef __cplusplus
extern "C"
{
#endif

ds18_t therm1;          /**< Define a ds18b20 device */

/**
 * @brief  Struct of basic params of ds18b20
 */

static ds18_params_t therm1_params = {
    .pin = GPIO14,               /*!< Assign the ds18b20 pin */
    .out_mode = GPIO_OD_PU,      /*!< ds18b20 pin mode_out */
    .in_mode = GPIO_IN
};

/**
 * @brief   Initialize a ds18 device
 *
 * @param[out] dev          device descriptor
 * @param[in]  params       ds18 initialization struct
 *
 * @return                   0 on success
 * @return                  -1 on error
 */
int ds18_init(ds18_t *dev, const ds18_params_t *params);

/**
 * @brief   convenience function for triggering a conversion and reading the
 * value
 *
 *
 * @param[in]  dev           device descriptor
 * @param[out] temperature  buffer to write the temperature in centi-degrees
 *
 * @return                   0 on success
 * @return                  -1 on error
 */
int ds18_get_temperature(const ds18_t *dev, int16_t *temperature);

#ifdef __cplusplus
}
#endif
#endif /* DS18B20_H */
/** @} */
