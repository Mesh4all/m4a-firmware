/*
 * Copyright (C) 2022 Mesh4all <mesh4all.org>
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
 *
 * @{
 * @brief       Cbor encoder and decoder functions to manipulate a group of data.
 *
 * @author      xkevin190 <kevinvelasco190@gmail.com>
 * @author      eduazocar <eduardo@turpialdev.com>
 *
 */

#include "esp_err.h"

#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief method to encode data
 * @param[in] key  Make a name to an element of map.
 * @param[in] value Encode a value int64 that belongs to [key named element]
 * @param[out] cbor_coded Buffer of coded message in cbor, \
 *                             this contains all elements that will be saved
 * @param[out] len buffer size.
 *
 * Format of element to be Encoded:
 * { "key_named" : value };
 *
 */

esp_err_t encode(char *key, int64_t value, uint8_t *cbor_coded, size_t *len);

/**
 * @brief method to encode data
 * @param[in] key  Make a name to an element of map.
 * @param[in] value Encode a value uint8_t or uint8* that belongs to [key named element]
 * @param[out] cbor_coded Buffer of coded message in cbor, \
 *                             this contains all elements that will be saved
 * @param[out] len buffer size.
 *
 * Format of element to be Encoded:
 * { "key_named" : 1523 };
 * TO understand could see these groups of bytes as string.
 *
 * { "key_name" : "a5f/s*"}
 *
 * @note: Take careful to don't send any char or string under uint8_t and uint8_t* types
 * this function could be take like an array of bytes.
 *
 */

esp_err_t encode_uint8(char *key, uint8_t *value, uint8_t *cbor_coded, size_t *len);

/**
 * @brief method to decode data
 * @param[in]  buf This is received data encoded under cbor
 * @param[in]  key Name of element to be decoded, if name is not defined, receive no value
 * @param[in]  len This is the size of buf received
 * @param[out] value Value referred of map element [key_named]
 */

esp_err_t decode(uint8_t *buf, char *key, void *value, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* SERIALIZATION_H */
/**@}*/
