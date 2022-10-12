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
 * @brief       This is the storage module where you can save data in the flash memory.
 *              This data should not be greater than 64 bytes. Data formats allowed are string
 *              and uint8.
 *
 * @author      xkevin190 <kevinvelasco193@gmail.com>
 * @author      eduazocar <eduazocarv@gmail.com>
 *
 */
#ifndef STORAGE_H
#define STORAGE_H

#include "mtd_flashpage.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function initializes all components needed to start and to save the data
 *
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int8_t mtd_start(void);

/**
 * @brief Saves any value in any position of mtd_storage.
 *
 * @param [in] value Any type of value.
 * @param [in] len  size of @p value that will be saved.
 * @param [in] offset reference to an position in mtd_storage.
 * @return int8_t
 */
int mtd_save(const void *value, uint32_t len, uint32_t offset);

/**
 * @brief Loads any value in any position of mtd_storage.
 *
 * @param [out] value Any type of value.
 * @param [in] len size of @p value that will be loaded.
 * @param [in] offset reference to an position in mtd_storage.
 * @return int8_t
 */
int mtd_load(void *value, uint16_t len, uint32_t offset);

int8_t mtd_erase_all(void);

/**
 * @brief Dump all data inside mtd storage
 *
 * @warning The mtd storage needs to define a flashpage limit after base program
 * memory allocation
 *
 * @retval 0 Exists data inside of mtd storage.
 * @retval -1 All data inside of mtd is erased.
 */
int8_t mtd_dump(void);

/**
 * @brief Dump all data inside an flshpage from mtd_storage
 *
 * @param page
 * @return int8_t
 */
int8_t mtd_dump_flashpage(uint16_t page);

/* Unsigned int put/get operations */

/**
 * @brief Saves an unsigned int 8 bits data type in the mtd_storage.
 *
 * @param [in] value uint8_t data.
 * @param [in] key An unique identifier string for the uint8_t data @p value.
 * @return int
 */
int mtd_put_u8(const uint8_t value, const uint8_t *key);
/**
 * @brief Saves an unsigned int 16 bits data type in the mtd_storage.
 *
 * @param [in] value uint16_t data.
 * @param [in] key An unique identifier string for the uint16_t data @p value.
 * @return int
 */
int mtd_put_u16(const uint16_t value, const uint8_t *key);
/**
 * @brief Saves an unsigned int 32 bits data type in the mtd_storage.
 *
 * @param [in] value uint32_t data.
 * @param [in] key An unique identifier string for the uint32_t data @p value.
 * @return int
 */
int mtd_put_u32(const uint32_t value, const uint8_t *key);
/**
 * @brief Load an unsigned int 8 bits data type in the mtd_storage
 *
 * @param [out] value uint8_t data variable that will load the value from mtd_storage
 * @param [in] key An unique identifier string for the uint8_t data @p value.
 * @return int
 */
int mtd_get_u8(uint8_t *value, const uint8_t *key);
/**
 * @brief Load an unsigned int 16 bits data type in the mtd_storage
 *
 * @param [out] value uint16_t data variable that will load the value from mtd_storage
 * @param [in] key An unique identifier string for the uint16_t data @p value.
 * @return int
 */
int mtd_get_u16(uint16_t *value, const uint8_t *key);
/**
 * @brief Load an unsigned int 32 bits data type in the mtd_storage
 *
 * @param [out] value uint32_t data variable that will load the value from mtd_storage
 * @param [in] key An unique identifier string for the uint32_t data @p value.
 * @return int
 */
int mtd_get_u32(uint32_t *value, const uint8_t *key);

/* Int put/set operations */

/**
 * @brief Saves a signed int 8 bits data type in the mtd_storage.
 *
 * @param [in] value int8_t data.
 * @param [in] key An unique identifier string for the int8_t data @p value.
 * @return int
 */
int mtd_put_i8(const int8_t value, const uint8_t *key);
/**
 * @brief Saves an signed int 16 bits data type in the mtd_storage.
 *
 * @param [in] value uint16_t data.
 * @param [in] key An unique identifier string for the uint16_t data @p value.
 * @return int
 */
int mtd_put_i16(const int16_t value, const uint8_t *key);
/**
 * @brief Saves an signed int 32 bits data type in the mtd_storage.
 *
 * @param [in] value uint32_t data.
 * @param [in] key An unique identifier string for the uint32_t data @p value.
 * @return int
 */
int mtd_put_i32(const int32_t value, const uint8_t *key);
/**
 * @brief Load an signed int 8 bits data type in the mtd_storage
 *
 * @param [out] value int8_t data variable that will load the value from mtd_storage
 * @param [in] key An unique identifier string for the int8_t data @p value.
 * @return int
 */
int mtd_get_i8(int8_t *value, const uint8_t *key);
/**
 * @brief Load an unsigned int 16 bits data type in the mtd_storage
 *
 * @param [out] value int16_t data variable that will load the value from mtd_storage
 * @param [in] key An unique identifier string for the int16_t data @p value.
 * @return int
 */
int mtd_get_i16(int16_t *value, const uint8_t *key);
/**
 * @brief Load an unsigned int 32 bits data type in the mtd_storage
 *
 * @param [out] value uint32_t data variable that will load the value from mtd_storage
 * @param [in] key An unique identifier string for the uint32_t data @p value.
 * @return int
 */
int mtd_get_i32(int32_t *value, const uint8_t *key);

/* String put/set operations */

/**
 * @brief Saves a string with its identifier.
 * @note it needs to specify the data string length.
 *
 * @param [in] value pointer to a string, points to a data string
 * @param [in] key An unique identifier string for the string data @p value.
 * @param [in] len size of @p value string.
 * @return int
 */
int mtd_put_str(const char *value, const uint8_t *key, uint8_t len);
/**
 * @brief Loads a string using its identifier.
 *
 * @param [out] value pointer to a string, points to a data string
 * @param [in] key An unique identifier string for the string data @p value.
 * @param [in] len size of @p value string.
 * @return int
 */
int mtd_get_str(char *value, const uint8_t *key, uint8_t len);

#ifdef __cplusplus
}
#endif
#endif /* STORAGE_H */
/** @} */
