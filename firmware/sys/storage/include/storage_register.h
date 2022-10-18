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
 * @ingroup     storage_module
 * @{
 * @file
 * @brief       This is the storage module where you can save data in the flash memory by registers
 *              you can add an identifier o key to locate a specify data.
 *
 * @author      eduazocar <eduazocarv@gmail.com>
 *
 */
#ifndef STORAGE_REGISTER_H
#define STORAGE_REGISTER_H

#include "mtd_flashpage.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MTD_REGISTER_INDEX_LIMIT (512) /*!< Reference to max number of bytes to add an register*/
#define RWP_WRITE_BITMASK (0x80) /*!< Read/Write protection bit mask to set Writeable register*/
#define RWP_READ_BITMASK (0x00)  /*!< Read/Write protection bit mask to set only-read register*/
typedef struct {
    uint16_t size;        /*!< saved data size*/
    char key[25];         /*!< name or identifier assigned a register where there is a saved dara */
    uint8_t rwp;          /*!< Read and write protection bit */
    uint32_t ptr_content; /*!< pointer to the content of a register */
} mtd_register_t;

#define MTD_REG_IDX_NUMOF                                                                          \
    (MTD_REGISTER_INDEX_LIMIT /                                                                    \
     sizeof(mtd_register_t)) /*!< Max number of indexes that can be saved in the mtd storage  */

/**
 * @brief Saves data using a key as identifier to localize a group of bytes.
 *
 * @param [in] value Any type of value.
 * @param [in] key Identifier to set/get a specified value.
 * @param [in] len size of @p value that will be saved.
 * @return int8_t
 */
int mtd_save_reg(const void *value, const char *key, uint16_t len);

/**
 * @brief Load data from mtd_storage localizating with its knew key and saves in @p value the data.
 *
 * @param [out] value Any type of value.
 * @param [in] key   Identifier to set/get a specified value.
 * @param [in] len  size of data that will be saved in @p value .
 *
 * @note @p size is used to check with the data saved in mtd_storage and won't be overflowed the @p
 * value parameter.
 * @return int8_t
 */
int mtd_load_reg(void *value, const char *key, uint16_t len);

/**
 * @brief Removes all saved data in mtd_storage. This will erase all until the
 * last page since to available flash page to write eeprom.
 *
 * @warning The mtd storage needs to define a flashpage limit after base program
 * memory allocation
 *
 * @retval 0 Erased data success
 * @retval -1 Erased Fail.
 */
int8_t mtd_reg_del(char *key, uint16_t size);

/**
 * @brief
 *
 * @param[inout] idx First available index to write a register.
 * @param[inout] count Number of available registers to write.
 * @return int8_t
 */
int8_t mtd_available_idx(uint8_t *idx, uint8_t *count);

/**
 * @brief Removes all registers and set all their read/write protection bit, making that registers
 * writeable
 *
 * @return int
 */
int mtd_clear_all_regs(void);

#ifdef __cplusplus
}
#endif
#endif /* STORAGE_REGISTER_H */
/** @} */
