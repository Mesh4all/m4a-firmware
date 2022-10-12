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
 * @brief       Storage registers file
 *
 * @author      eduazocar <eduazocarv@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errno.h"

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

int mtd_clear_all_regs(void) {
    mtd_register_t reg;
    memset(&reg, 0xFF, sizeof(reg));
    reg.rwp &= RWP_WRITE_BITMASK;
    mtd_erase_all();
    uint32_t addr = MTD_START_ADDR + sizeof(reg.size) + sizeof(reg.key);
    for (size_t i = 0; i < MTD_REG_IDX_NUMOF; i++) {
        if (mtd_save(&reg.rwp, sizeof(reg.rwp), addr) < 0) {
            DEBUG("Error dEL REGS\n");
            return -1;
        }
        addr += sizeof(mtd_register_t);
    }
    return 0;
}

int mtd_save_reg(const void *value, const uint8_t *key, uint16_t len) {
    mtd_register_t buff, mtd_reg = {.size = len};
    memcpy(mtd_reg.key, key, sizeof(mtd_reg.key));
    mtd_reg.rwp = 0xFF & RWP_READ_BITMASK;
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

int8_t mtd_available_idx(uint8_t *idx, uint8_t *count) {
    (void)idx;
    mtd_register_t buff;
    *count = 0;

    for (size_t i = 0; i < MTD_REG_IDX_NUMOF; i++) {
        uint32_t idx_storage = MTD_START_ADDR + i * sizeof(mtd_register_t);
        mtd_load(&buff, sizeof(buff), idx_storage);
        if (idx_reg_is_empty(buff) == 0) {
            DEBUG("Available index: 0x%" PRIX32 "\n", idx_storage);
            if (*count != 0) {
                *idx = idx_storage;
            }
            *count += 1;
        }
    }
    if (*count == 0) {
        DEBUG("It's not available register for write\n");
        return -1;
    }
    return 0;
}

int8_t mtd_reg_del(uint8_t *key, uint16_t size) {
    mtd_register_t regs[MTD_REG_IDX_NUMOF], reg = {.size = size};
    memcpy(reg.key, key, sizeof(reg.key));
    uint8_t *content[MTD_REG_IDX_NUMOF];
    uint8_t j = 0;
    for (size_t i = 0; i < MTD_REG_IDX_NUMOF; i++) {
        int8_t reg_st = 0;
        uint32_t idx_storage = MTD_START_ADDR + i * sizeof(mtd_register_t);
        mtd_load(&regs[i - j], sizeof(mtd_register_t), idx_storage);
        reg_st = check_idx_reg(regs[i - j], reg);
        if (reg_st == 0) {
            j++;
        } else if (reg_st == -1) {
            content[i - j] = malloc(regs[i - j].size);
            mtd_load(content[i - j], regs[i - j].size, regs[i - j].ptr_content);
            printf("\n");
        }
    }
    mtd_clear_all_regs();
    for (size_t i = 0; i < MTD_REG_IDX_NUMOF; i++) {
        if (idx_reg_is_empty(regs[i]) < 0) {
            mtd_save_reg(content[i], regs[i].key, regs[i].size);
            free(content[i]);
        } else {
            break;
        }
    }
    return 0;
}
