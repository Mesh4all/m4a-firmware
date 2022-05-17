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
 * @ingroup     serialization
 * @{
 * @file
 * @brief       Serialization implementation for this will use tinycbor
 *
 * @author      xkevin190 <kevinvelasco190@gmail.com>
 *
 */
#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This is an struct used to save the sensor data.
 *
 */
typedef struct {
    int16_t temperature; /*!< temperature value */
    int soil_moisture;   /*!< moisture value */
    char id_node[64];    /*!< node id */
} sensor_data;

/**
 * @brief  This function decodes the arriving message from udp
 * @param [in]   buffer  data codified
 * @param [out]  data    output of the decodified data
 * @param [in]   length  length of the data
 * @return  0 Satisfactory result
 *         -1 Failed result
 */
int cbor_decode_message(uint8_t *buffer, sensor_data *data, size_t length);

/**
 * @brief  This function decodes a message before sending it to udp client
 * @param [in]   data  sensors values
 * @param [out]  output  output of the codified data
 * @param [in]   len_output  length of the data
 * @return  0 Satisfactory result
 *         -1 Failed result
 */
int cbor_enconde_message(sensor_data *data, uint8_t *output, size_t *len_output);

#ifdef __cplusplus
}
#endif
#endif /* SERIALIZATION_H */
/** @} */
