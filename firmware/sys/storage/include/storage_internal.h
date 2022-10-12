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
#ifndef STORAGE_INTERNAL_H
#define STORAGE_INTERNAL_H

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

#ifdef __cplusplus
}
#endif
#endif /* STORAGE_INTERNAL_H */
/** @} */
