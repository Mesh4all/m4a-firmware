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
 * @ingroup  tests_uniqueid
 * @{
 *
 * @file
 * @brief   Randomizer to obtain values hex in binary
 *
 * @author  eduazocar <eduazocarv@gmail.com>
 */
#ifndef UNIQUE_RANDOM_H
#define UNIQUE_RANDOM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Random to gets a block of random values
 *
 * @param bit_ref this is a reference to create a matrix of the reads
 * e.g bit_ref = 9 -> 9x9 (This mean that could obtain 9 measures of 9
 * random bits generated )
 */
void random_generator(uint8_t bit_ref);

/**
 * @brief   Printer to random values
 *
 * @param val Any input value in the range of 32 bits
 * @param len length of the bits int @p val
 */
void print_binary(uint32_t val, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif /* UNIQUE_RANDOM_H */
/** @} */
