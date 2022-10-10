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

int8_t mtd_start(void) {
    int ret = mtd_init(dev);
    if (ret < 0) {
        DEBUG("Error to the init mtd \n");
    }
    return ret;
}

int mtd_write_block(const void *val, uint32_t addr, uint8_t size, uint8_t offset) {
    uint8_t buf[dev->write_size];
    int8_t ret = 0;
    ret = mtd_read(dev, buf, addr, sizeof(buf));
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

int8_t idx_reg_is_empty(mtd_register_t reg) {
    uint8_t empty_reg[sizeof(mtd_register_t)];
    memset(empty_reg, FLASHPAGE_ERASE_STATE, sizeof(empty_reg));
    if (memcmp(&reg, empty_reg, sizeof(reg.size) + sizeof(reg.key)) == 0) {
        return 0;
    }
    return -1;
}

int8_t check_idx_reg(mtd_register_t reg, mtd_register_t buffer) {
    if (memcmp(&reg, &buffer, sizeof(reg.size) + sizeof(reg.key)) == 0) {
        return 0;
    }
    if (idx_reg_is_empty(reg) == 0) {
        return 1;
    }
    return -1;
}

int mtd_save_reg(const void *value, const uint8_t *key, uint16_t len) {
    mtd_register_t buff, mtd_reg = {.size = len};
    memcpy(mtd_reg.key, key, sizeof(mtd_reg.key));
    uint8_t reg_count = 0;
    mtd_reg.ptr_content = MTD_START_ADDR + MTD_REGISTER_INDEX_LIMIT;
    while (reg_count < MTD_REG_IDX_NUMOF) {
        int8_t ret = 0;

        int8_t reg_state = 0;
        ret = mtd_load(&buff, sizeof(mtd_register_t),
                       MTD_START_ADDR + (reg_count * sizeof(mtd_register_t)));
        if (ret < 0) {
            DEBUG("Failed to pre-load indexes, file: %s, line %d, function: %s\n", __FILE__,
                  __LINE__, __func__);
            return ret;
        }
        if (buff.size != 0xffff) {
            mtd_reg.ptr_content += buff.size;
        }
        reg_state = check_idx_reg(buff, mtd_reg);
        if (reg_state == 0) {
            DEBUG("The index already exist, can´t be updated, file: %s, line %d, function: %s\n",
                  __FILE__, __LINE__, __func__);
            return 1;
        }
        if (reg_state == 1) {
            DEBUG("Empty_register\n");
            ret = mtd_save(&mtd_reg, sizeof(mtd_register_t),
                           MTD_START_ADDR + (reg_count * sizeof(mtd_register_t)));
            if (ret < 0) {
                DEBUG("Failed Saving mtd register index, file: %s, line %d, function: %s\n",
                      __FILE__, __LINE__, __func__);
                return ret;
            }
            ret = mtd_save(value, len, mtd_reg.ptr_content);
            if (ret < 0) {
                DEBUG("Failed Saving mtd register content, file: %s, line %d, function: %s\n",
                      __FILE__, __LINE__, __func__);
                return ret;
            }
            return 0;
        }
        reg_count++;
    }
    return -1;
}

int mtd_load_reg(void *value, const uint8_t *key, uint16_t len) {
    mtd_register_t buff, mtd_reg = {.size = len};
    memcpy(mtd_reg.key, key, sizeof(mtd_reg.key));
    uint8_t reg_count = 0;
    while (reg_count < MTD_REG_IDX_NUMOF) {
        int8_t ret = 0;
        int8_t reg_state = 0;
        ret = mtd_load(&buff, sizeof(mtd_register_t),
                       reg_count * sizeof(mtd_register_t) + MTD_START_ADDR);
        if (ret < 0) {
            DEBUG("Err: Failed Reading Registers, file: %s, line %d, function: %s\n", __FILE__,
                  __LINE__, __func__);
            return ret;
        }

        reg_state = check_idx_reg(buff, mtd_reg);
        if (reg_state == 0) {
            DEBUG("Reading content of an register\n");
            ret = mtd_load(&mtd_reg, sizeof(mtd_register_t),
                           reg_count * sizeof(mtd_register_t) + MTD_START_ADDR);
            if (ret < 0) {
                return ret;
            }

            uint8_t out[mtd_reg.size];
            if (mtd_load(out, mtd_reg.size, mtd_reg.ptr_content) < 0) {
                return -1;
            }
            memcpy(value, out, len);
            return 0;
        }
        reg_count++;
    }
    return -1;
}

int mtd_put_u8(const uint8_t value, const uint8_t *key) {
    return mtd_save_reg(&value, key, sizeof(uint8_t));
}

int mtd_put_u16(const uint16_t value, const uint8_t *key) {
    return mtd_save_reg(&value, key, sizeof(uint16_t));
}

int mtd_put_u32(const uint32_t value, const uint8_t *key) {
    return mtd_save_reg(&value, key, sizeof(uint32_t));
}

int mtd_get_u8(uint8_t *value, const uint8_t *key) {
    return mtd_load_reg(value, key, sizeof(uint8_t));
}

int mtd_get_u16(uint16_t *value, const uint8_t *key) {
    return mtd_load_reg(value, key, sizeof(uint16_t));
}

int mtd_get_u32(uint32_t *value, const uint8_t *key) {
    return mtd_load_reg(value, key, sizeof(uint32_t));
}

int mtd_put_i8(const int8_t value, const uint8_t *key) {
    return mtd_save_reg(&value, key, sizeof(int8_t));
}

int mtd_put_i16(const int16_t value, const uint8_t *key) {
    return mtd_save_reg(&value, key, sizeof(int16_t));
}

int mtd_put_i32(const int32_t value, const uint8_t *key) {
    return mtd_save_reg(&value, key, sizeof(int32_t));
}

int mtd_get_i8(int8_t *value, const uint8_t *key) {
    return mtd_load_reg(value, key, sizeof(int8_t));
}

int mtd_get_i16(int16_t *value, const uint8_t *key) {
    return mtd_load_reg(value, key, sizeof(int16_t));
}

int mtd_get_i32(int32_t *value, const uint8_t *key) {
    return mtd_load_reg(value, key, sizeof(int32_t));
}

int mtd_put_str(const char *value, const uint8_t *key, uint8_t len) {
    return mtd_save_reg(value, key, len);
}

int mtd_get_str(char *value, const uint8_t *key, uint8_t len) {
    return mtd_load_reg(value, key, len);
}

int8_t mtd_available_idx(void) {
    mtd_register_t buff;
    uint8_t available_reg = 0;

    for (size_t i = 0; i < MTD_REG_IDX_NUMOF; i++) {
        uint32_t idx_storage = MTD_START_ADDR + i * sizeof(mtd_register_t);
        mtd_load(&buff, sizeof(buff), idx_storage);
        if (idx_reg_is_empty(buff) == 0) {
            DEBUG("Available index: 0x%" PRIX32 "\n", idx_storage);
            available_reg++;
        }
    }
    if (available_reg == 0) {
        DEBUG("It's not available register for write\n");
        return -1;
    }

    DEBUG("Number of available registers: %u\n\n", available_reg);
    return 0;
}

int8_t mtd_erase_all(void) {
    uint32_t addr = MTD_START_ADDR;
    if (mtd_erase(dev, addr, MAX_SIZE_STORAGE) < 0) {
        return -1;
    }
    return 0;
}

int8_t mtd_dump(void) {
    uint8_t value[dev->write_size];
    uint32_t addr = MTD_START_ADDR;
    uint32_t erased_data = 0;
    for (uint32_t i = 0; i < MAX_SIZE_STORAGE / dev->write_size; i++) {
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
