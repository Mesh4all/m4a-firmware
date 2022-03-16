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
 * @brief       this is module of storage this module save data in the flash memory
 *              this data could not be greater than 64 bytes, you can save data only like string
 *              and uint8
 *
 * @author      xkevin190 <kevinvelasco193@gmail.com>
 *
 */
#ifndef STORAGE_H
#define STORAGE_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SIZE_STORAGE 16 /*!< max size to save in the page */

/**
 * @brief this function init all need for start to save the data
 *
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int mtd_start(void);

/**
 * @brief this function is executed by mtd write_string and mtd write uint8 internally it is not
 * recommended to use it directly, use the functions mentioned above
 *
 * @param [in] key  this is the address where will be save the data in the memory
 * @param [out] value value to save
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int mtd_save(uint32_t key, void *value);

/**
 * @brief function used to get the strings already saved
 *
 * @param [in] key  this is the address where will be save the data in the memory
 * @param [out] value value to save
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int mtd_read_string(uint32_t key, char *output, size_t len);

/**
 * @brief function used to get the uint8 values already saved
 *
 * @param [in] key  this is the address where will be save the data in the memory
 * @param [out] value value to save
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int mtd_read_u8(uint32_t key, uint8_t *output);

/**
 * @brief this function deletes the saved data
 *
 * @param key this is the address where will be removed the data in the memory
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int mtd_erase_flashpage(uint32_t key);

/**
 * @brief function used for write only strings if it are greater than 64 bytes your string will be
 * cut and will save only 64 bytes
 *
 * @param [in] key  this is the address where will be save the data in the memory
 * @param [out] value value to save
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int mtd_write_string(uint32_t key, char *value);

/**
 * @brief function used for write only uint8_t
 *
 * @param [in] key  this is the address where will be save the data in the memory
 * @param [out] value value to save
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int mtd_write_uint8(uint32_t key, uint8_t *value);

/**
 * @brief function used for get the length of strings
 *
 * @param [in] key  this is the address of memory
 *
 */
int mtd_get_string_len(uint32_t key);

#ifdef __cplusplus
}
#endif
#endif /* STORAGE_H */
/** @} */
