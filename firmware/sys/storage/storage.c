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
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "shell_commands.h"
#include "msg.h"
#include "system_init.h"

#include "log.h"
#include "board.h"
#include "mtd.h"
#include "mtd_flashpage.h"
#include "storage.h"

static mtd_dev_t _dev = MTD_FLASHPAGE_INIT_VAL(8);
static mtd_dev_t *dev = &_dev;

int mtd_start(void) {
    int ret = mtd_init(dev);
    if (ret < 0) {
        printf("Error to the init mtd \n");
    }
    return ret;
}

int mtd_save(uint32_t key, void *value) {
    uint8_t buf[16];

    memset(buf, 0, sizeof(buf));
    memcpy(buf, value, sizeof(buf));
    int ret = mtd_write(dev, buf, key, sizeof(buf));

    if (ret < 0) {
        printf("Error to the write in mtd %d \n", ret);
    }

    return ret;
}

int mtd_read_u8(uint32_t key, uint8_t *output) {
    size_t len = MAX_SIZE_STORAGE;
    uint8_t buf_read[len];
    memset(buf_read, 0, sizeof(buf_read));

    int ret = mtd_read(dev, buf_read, key, sizeof(buf_read));
    if (ret < 0) {
        printf("Error to read in mtd \n");
    }

    *output = buf_read[0];

    return ret;
}

int mtd_get_string_len(uint32_t key) {
    size_t len = MAX_SIZE_STORAGE;
    char buf_read[len];
    memset(buf_read, 0, sizeof(buf_read));

    int ret = mtd_read(dev, buf_read, key, sizeof(buf_read));
    if (ret < 0) {
        printf("Error to read in mtd \n");
        return 0;
    }
    int count = 0;
    for (size_t i = 0; i < len; i++) {
        printf("%x ", buf_read[i]);
        if (buf_read[i] == 0 && count == 0) {
            count = i;
        }
    }
    printf("\n ");
    return count + 1;
}

int mtd_read_string(uint32_t key, char *output, size_t len) {

    size_t _len = len;
    char buf_read[_len];
    memset(buf_read, 0, sizeof(buf_read));

    int ret = mtd_read(dev, buf_read, key, sizeof(buf_read));
    if (ret < 0) {
        printf("Error to read in mtd \n");
    }
    int count = 0;
    for (size_t i = 0; i < len; i++) {
        if (buf_read[i] == 0 && count == 0) {
            count = i;
        }
    }

    memcpy(output, buf_read, count + 1);

    return ret;
}

int mtd_erase_flashpage(uint32_t key) {
    /* Erase last sector */
    int ret = mtd_erase(dev, key, FLASHPAGE_SIZE);
    if (ret < 0) {
        printf("Error to the erase the address \n");
    }

    return ret;
}

int mtd_write_string(uint32_t key, char *value) {
    int err = mtd_erase_flashpage(key);
    if (err < 0) {
        printf("Error to the erase the address \n");
        return err;
    }

    err = mtd_save(key, value);
    if (err < 0) {
        printf("Error to the erase the address \n");
        return err;
    }

    return 0;
}

int mtd_write_uint8(uint32_t key, uint8_t *value) {
    int err = mtd_erase_flashpage(key);
    if (err < 0) {
        printf("Error to the erase the address \n");
        return err;
    }

    err = mtd_save(key, value);
    if (err < 0) {
        printf("Error to the erase the address \n");
        return err;
    }

    return 0;
}
