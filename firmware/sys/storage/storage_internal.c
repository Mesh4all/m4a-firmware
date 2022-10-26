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
#include <string.h>
#include "errno.h"

#include "mtd.h"
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

#define MTD_WR_BLOCK_POS(offset) ((offset - MTD_START_ADDR) / dev->write_size)
#define MTD_WR_NUMOF_BLOCK(size, offset) ((size) + (offset)) / (dev->write_size)
#define MTD_LAST_BLOCK_RES(size, offset) (offset) + (size) % (dev->write_size)

int mtd_write_block(const void *val, uint32_t addr, uint8_t size, uint8_t offset) {
    uint8_t buf[dev->write_size];
    int8_t ret = 0;
    ret = mtd_read(dev, buf, addr, dev->write_size);
    if (ret < 0) {
        DEBUG("Err: Reading a Block before overwrite it\n");
        return ret;
    }
    memcpy(buf + offset, val, size - offset);
    ret = mtd_write(dev, buf, addr, dev->write_size);
    if (ret < 0) {
        DEBUG("Err: Writing in a block\n");
        return ret;
    }
    return 0;
}

int mtd_read_block(void *value, uint32_t addr, uint8_t size, uint8_t offset) {
    uint8_t buf[dev->write_size];
    int8_t ret = 0;
    memset(buf, 0xFF, sizeof(buf));
    ret = mtd_read(dev, buf, addr, dev->write_size);
    if (ret < 0) {
        DEBUG("Err: Reading a Block\n");
        return ret;
    }
    memcpy(value, buf + offset, size - offset);
    return 0;
}

int mtd_save(const void *value, uint32_t len, uint32_t addr) {
    uint8_t block_offset = addr % dev->write_size;
    uint8_t last_size = MTD_LAST_BLOCK_RES(len, block_offset);
    uint32_t storage_addr = MTD_START_ADDR + MTD_WR_BLOCK_POS(addr) * dev->write_size;
    if (len + storage_addr >= MTD_LAST_ADDR) {
        DEBUG("error: Overload Memory size, file: %s, line %d, function: %s\n", __FILE__, __LINE__,
              __func__);
        return -EOVERFLOW;
    }
    for (uint16_t i = 0; i <= MTD_WR_NUMOF_BLOCK(len, block_offset); i++) {
        if (mtd_write_block(value, storage_addr,
                            i == MTD_WR_NUMOF_BLOCK(len, block_offset) ? last_size
                                                                       : dev->write_size,
                            block_offset) < 0) {
            DEBUG("error: Writing block, file: %s, line %d, function: %s\n", __FILE__, __LINE__,
                  __func__);
            return -EOVERFLOW;
        }
        value = (uint8_t *)value + dev->write_size - block_offset;
        block_offset = 0;
        storage_addr += dev->write_size;
    }
    return 0;
}

int mtd_load(void *value, uint16_t len, uint32_t addr) {
    uint8_t block_offset = addr % dev->write_size;
    uint8_t last_size = MTD_LAST_BLOCK_RES(len, block_offset);
    uint32_t storage_addr = MTD_START_ADDR + MTD_WR_BLOCK_POS(addr) * dev->write_size;

    if (len + addr >= MTD_LAST_ADDR) {
        DEBUG("error: Overload Memory size, file: %s, line %d, function: %s\n", __FILE__, __LINE__,
              __func__);
        return -EOVERFLOW;
    }
    for (uint16_t i = 0; i <= MTD_WR_NUMOF_BLOCK(len, block_offset); i++) {
        if (mtd_read_block(value, storage_addr,
                           i == MTD_WR_NUMOF_BLOCK(len, block_offset) ? last_size : dev->write_size,
                           block_offset) < 0) {
            DEBUG("Error: Reading in block, file: %s, line %d, function: %s\n", __FILE__, __LINE__,
                  __func__);
            return -EOVERFLOW;
        }
        value = (uint8_t *)value + dev->write_size - block_offset;
        block_offset = 0;
        storage_addr += dev->write_size;
    }
    return 0;
}

int8_t mtd_dump(void) {
    uint8_t value[dev->write_size];
    uint32_t addr = MTD_START_ADDR;
    uint32_t erased_data = 0;
    for (uint32_t i = 0; i < MAX_SIZE_STORAGE / dev->write_size; i++) {
        DEBUG("0x%" PRIX32 " : ", addr);
        mtd_read_block(value, addr, dev->write_size, 0);
        for (uint8_t j = 0; j < dev->write_size; j++) {
            DEBUG("%02X ", value[j]);
            if ((value[j] == 255) || (value[j] == 0)) {
                erased_data++;
            }
        }
        DEBUG("\n");
        addr += dev->write_size;
    }
    if (erased_data == MAX_SIZE_STORAGE) {
        return -1;
    }
    return 0;
}
