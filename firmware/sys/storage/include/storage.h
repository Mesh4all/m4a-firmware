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

#define LAST_AVAILABLE_PAGE (FLASHPAGE_NUMOF - 1) /*!< Last position in the block EEPROM*/
#define MAX_SIZE_STORAGE (4096)                   /*!< max size of mtd_storage, only writeable 4K */
#define MAX_NUMOF_FLASHPAGES                                                                       \
    (MAX_SIZE_STORAGE / FLASHPAGE_SIZE) /*!< max num of pages that can be manipulated */

#define MTD_START_ADDR                                                                             \
    ((uint32_t)(flashpage_addr(LAST_AVAILABLE_PAGE)) -                                             \
     MAX_SIZE_STORAGE) /*!< Reference to the first address writeable*/
#define MTD_LAST_ADDR                                                                              \
    (uint32_t)(flashpage_addr(LAST_AVAILABLE_PAGE)) /*!< Reference to the last address writeable*/
#define MTD_REGISTER_INDEX_LIMIT (512) /*!< Reference to max number of bytes to add an register*/

typedef struct {
    uint16_t size;
    uint8_t key[25];
    uint8_t rwp;
    uint32_t ptr_content;
} mtd_register_t;

#define MTD_REG_IDX_NUMOF (MTD_REGISTER_INDEX_LIMIT / sizeof(mtd_register_t)) /*!<  */

/** @note The storage EEPROM section page could be resize with the bootloader block size
 *  @warning Always the block EEPROM and BOOTLOADER are affected between them.
 */
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

/**
 * @brief Saves data using a key as identifier to localize a group of bytes.
 *
 * @param [in] value Any type of value.
 * @param [in] key Identifier to set/get a specified value.
 * @param [in] len size of @p value that will be saved.
 * @return int8_t
 */
int mtd_save_reg(const void *value, const uint8_t *key, uint16_t len);

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
int mtd_load_reg(void *value, const uint8_t *key, uint16_t len);

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
int8_t mtd_erase_all(void);

/**
 * @brief Check the available registers, to write/save data.
 *
 * @return int8_t
 */
int8_t mtd_available_idx(void);

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
