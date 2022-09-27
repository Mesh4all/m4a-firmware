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

#include <stdio.h>
#include "mtd_flashpage.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LAST_AVAILABLE_PAGE (FLASHPAGE_NUMOF - 1) /*!< Last position in the block EEPROM*/
#define MAX_SIZE_STORAGE (FLASHPAGE_SIZE)         /*!< max size to save in the page */

#define MAX_NUMOF_FLASHPAGES (FLASHPAGE_NUMOF) /*!< max num of pages that can be manipulated */

#if CPU_SAMD21A
#undef MAX_NUMOF_FLASHPAGES
#define EEPROM_SIZE (FLASHPAGE_SIZE * FLASHPAGE_PAGES_PER_ROW * 64)
#define MAX_NUMOF_FLASHPAGES (EEPROM_SIZE / FLASHPAGE_SIZE)
#endif

/** @note The storage EEPROM section page could be resize with the bootloader block size
 *  @warning Always the block EEPROM and BOOTLOADER are affected between them.
 */
/**
 * @brief This function initializes all components needed to start and to save the data
 *
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int mtd_start(void);

/**
 * @brief This function is executed by mtd write_string and mtd_write_uint8 internally. It is not
 * recommended to use it directly, use the functions mentioned
 *
 * @param [in] key  this is the address where will be saved the data in the memory
 * @param [out] value value to save
 * @retval 0 Satisfactory result
 * @retval -1 Failed result
 */
int mtd_save(uint32_t key, void *value);

/**
 * @brief This function is executed to save any data type or any strutc, The function saves all the
 * information compressed in EEPROM/FLASH_MEMORY. The data is stored in each page of the
 * EEPROM/MEMORY_FLASH section.
 * @warning: The EEPROM/FLASH_MEMORY section size depends of itself hardware.
 *
 * @param [in]  value The input value given could be any datatype or data struct
 * @param [in]  len  size of the all elements in the container of @p value
 * @retval  0 Satisfactory result
 * @retval  -1 Failed result
 */
int mtd_save_compress(void *value, uint16_t len);

/**
 * @brief load all data in the storage, depending of the number of bytes ( @p len ) required to read
 * from the storage block in the mtd device.
 *
 * @param [out]  value pointer to an struct or variable where will be loaded the data of the
 * storage.
 * @param [in]  len  size of the struct or variable @p value
 * @retval  0 Satisfactory result
 * @retval  -1 Failed result
 */
int mtd_load(void *value, uint16_t len);

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
int mtd_erase_all(void);

/**
 * @brief Dump all data inside mtd storage
 *
 * @warning The mtd storage needs to define a flashpage limit after base program
 * memory allocation
 *
 * @retval 0 Exists data inside of mtd storage.
 * @retval -1 All data inside of mtd is erased.
 */
int mtd_dump(void);

/**
 * @brief Function used to get the strings already saved
 *
 * @param [in]  key    This is the address where will be saved the data in the memory
 * @param [in]  len    Length of the data
 * @param [out] output Output to save
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int mtd_read_string(uint32_t key, char *output, size_t len);

/**
 * @brief Function used to get the uint8 values already saved
 *
 * @param [in]  key    This is the address where will be saved the data in the memory
 * @param [out] output Output to save
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int mtd_read_u8(uint32_t key, uint8_t *output);

/**
 * @brief This function deletes the saved data
 *
 * @param key This is the address where will be removed the data in the memory
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int mtd_erase_flashpage(uint32_t key);

/**
 * @brief This function is used to write only strings.
 * If they are greater than 64 bytes your string will be
 * cut and will save only 64 bytes
 *
 * @param [in] key  This is the address where will be saved the data in the memory
 * @param [out] value Value to save
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int mtd_write_string(uint32_t key, char *value);

/**
 * @brief Function used to write only uint8_t
 *
 * @param [in] key  This is the address where will be saved the data in the memory
 * @param [out] value Value to save
 * @return  0 Satisfactory result
 *          -1 Failed result
 */
int mtd_write_uint8(uint32_t key, uint8_t *value);

/**
 * @brief Function used to get the length of strings
 *
 * @param [in] key  This is the address of memory
 *
 */
int mtd_get_string_len(uint32_t key);

#ifdef __cplusplus
}
#endif
#endif /* STORAGE_H */
/** @} */
