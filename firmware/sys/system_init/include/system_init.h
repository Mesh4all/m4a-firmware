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
 * @brief      this file init all peripherals and necessary dependencies for his correct work.
 *
 * @author     xkevin190 <kevinvelasco190@gmail.com>
 *
 */
#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief this struct content the status of th sensors
 */
typedef struct
{
   int ds18_is_available;                   /*!< variable used to check if the ds18 sensor
                                               is enabled */
   int moisture_sensor_is_available;        /*!< variable used to check if the soil moisture sensor
                                               is enabled */
} available_sensors_t;

/**
 * @brief this function init all peripherals and verify his connection status
 * also executes a thread where periodically get the peripherals values
 * and send to the reciver board
 *
 * @return  0 Satisfactory result
 *         -1 Failed result
 */
int init_initial_params (void);
/**
 * @brief this function set the wired ipv6 global address, it is generated
 * a through the mac of the hardware
 *
 * @return  0 Satisfactory result
 *         -1 Failed result
 */
int set_wired_ipv6 (void);

#ifdef __cplusplus
}
#endif
#endif /* SYSTEM_INIT_H */
/** @} */
