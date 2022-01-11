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
#ifndef MOISTURE_SENSOR_H
#define MOISTURE_SENSOR_H

#ifdef __cplusplus
extern "C"
{
#endif
/**
 * this a module created for hw390 moisture sensor this is an adc device,
 * for the correct func oof this module you will be use 3.3v and one resolution of 10bits
 *
 */

/**
 * @brief  this function initializes the moisture sensor
 * @return  0 satisfactory result
 *         -1 failed result
 */
int init_moisture(void);
/**
 * @brief  function to get the sensor data
 * @param[out]  output this variable will return the moisture value
 * @return  0 satisfactory result
 *         -1 failed result
 */
int get_moisture_value(int* output);

#ifdef __cplusplus
}
#endif
#endif /* MOISTURE_SENSOR_H */
/** @} */
