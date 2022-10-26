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
 * @brief       storage file
 *
 * @author      xkevin190 <kevinvelasco193@gmail.com>
 * @author      eduazocar <eduazocarv@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errno.h"

#include "mtd.h"
#include "storage.h"
#include "storage_register.h"
#include "storage_internal.h"

#if (CONFIG_DEBUG_STORAGE) || (DOXYGEN)
/**
 * @brief KCONFIG_PARAMETER TO SET DEBUG MODE
 *
 */
#define ENABLE_DEBUG CONFIG_DEBUG_STORAGE
#else
#define ENABLE_DEBUG 0
#endif
#include "debug.h"

static mtd_flashpage_t _dev = MTD_FLASHPAGE_INIT_VAL(8);
static mtd_dev_t *dev = &_dev.base;

int8_t mtd_start(void) {
    int ret = mtd_init(dev);
    if (ret < 0) {
        DEBUG("Error to the init mtd \n");
    }
    return ret;
}

int mtd_put_u8(const uint8_t value, const char *key) {
    return mtd_save_reg(&value, key, sizeof(uint8_t));
}

int mtd_put_u16(const uint16_t value, const char *key) {
    return mtd_save_reg(&value, key, sizeof(uint16_t));
}

int mtd_put_u32(const uint32_t value, const char *key) {
    return mtd_save_reg(&value, key, sizeof(uint32_t));
}

int mtd_get_u8(uint8_t *value, const char *key) {
    return mtd_load_reg(value, key, sizeof(uint8_t));
}

int mtd_get_u16(uint16_t *value, const char *key) {
    return mtd_load_reg(value, key, sizeof(uint16_t));
}

int mtd_get_u32(uint32_t *value, const char *key) {
    return mtd_load_reg(value, key, sizeof(uint32_t));
}

int mtd_put_i8(const int8_t value, const char *key) {
    return mtd_save_reg(&value, key, sizeof(int8_t));
}

int mtd_put_i16(const int16_t value, const char *key) {
    return mtd_save_reg(&value, key, sizeof(int16_t));
}

int mtd_put_i32(const int32_t value, const char *key) {
    return mtd_save_reg(&value, key, sizeof(int32_t));
}

int mtd_get_i8(int8_t *value, const char *key) {
    return mtd_load_reg(value, key, sizeof(int8_t));
}

int mtd_get_i16(int16_t *value, const char *key) {
    return mtd_load_reg(value, key, sizeof(int16_t));
}

int mtd_get_i32(int32_t *value, const char *key) {
    return mtd_load_reg(value, key, sizeof(int32_t));
}

int mtd_put_str(const char *value, const char *key, uint8_t len) {
    return mtd_save_reg(value, key, len);
}

int mtd_get_str(char *value, const char *key, uint8_t len) {
    return mtd_load_reg(value, key, len);
}

int8_t mtd_erase_all(void) {
    uint32_t addr = MTD_START_ADDR;
    if (mtd_erase(dev, addr, MAX_SIZE_STORAGE) < 0) {
        return -1;
    }
    return 0;
}
