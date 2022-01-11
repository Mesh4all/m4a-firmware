/*
 * Copyright (c) 2022 Mesh4all <mesh4all.org>
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
 * @ingroup     m4a-firmware
 * @{
 * @file
 * @brief       moisture sensor file
 *
 * @copyright   Copyright (c) 2021 Mesh for all
 * @author      xkevin190 <kevinvelasco190@gmail.com>
 *
 */
#ifndef DS18_SENSOR_H
#define DS18_SENSOR_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  This function initializes the temperature sensor
 * @param[in]  pin  Pin number where the data will be taken
 * @return  0 Satisfactory result
 *         -1 Failed result
 */
int init_temperature_sensor(int pin);

/**
 * @brief  Function to get the sensor data
 * @param[out]  output This variable will return the temperature value
 * @return  0 Satisfactory result
 *         -1 Sailed result
 */
int get_temperature_value(int16_t* output);

#ifdef __cplusplus
}
#endif
#endif /* DS18_SENSOR_H */
/** @} */
