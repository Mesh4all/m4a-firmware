/* 
 * Copyright (c) 2021 Mesh4all <mesh4all.org>
 
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
 * @author      luisan00 <luisan00@hotmail.com>
 */
#include <stdio.h>
#include <string.h>

#include "xtimer.h"
#include "shell.h"
#include "shell_commands.h"
#include "msg.h"
#include "periph/i2c.h"
#include "log.h"

// i2c touch panel
// bus   i2c1
// addr  0x54
// --
#define TOUCH_DEV I2C_DEV(0)
// i2c audio codec
// bus   i2c2
// addr  0x94
// speed Max. 100 KHz
#define SOUND_DEV I2C_DEV(1)
#define SOUND_ADDR 0x94
#define SND_CHIPID 0x01

int screen_control_init(int argc, char **argv)
{
    i2c_init(TOUCH_DEV);
    int res = i2c_acquire(TOUCH_DEV);
    if (res != 0)
    {
        printf("Can't acquire the given i2c\n");
        return -1;
    }

    return 0;
}

int init_sound_control(void)
{
    return 0;
}

int sound_control(int argc, char **argv)
{
    int res = i2c_acquire(SOUND_DEV);
    if (i2c_acquire(SOUND_DEV) < 0)
    {
        puts("can't acquire i2c");
        return res;
    }
    uint8_t chip_id;
    res = i2c_read_reg(SOUND_DEV, SOUND_ADDR, 0x10, &chip_id, 0);
    if (res != 0)
    {
        puts("cant read the chip id");
        return res;
    }
    printf("CHIPID; %c\n", chip_id);
    return 0;
}

static shell_command_t shell_extended_commands[] = {
    {"touch_id", "return chip id", sound_control},
    {NULL, NULL, NULL}};

int main(void)
{
    printf("~~ Welcome to Mesh4all ~~\n");
    /* Start shell */

    char line_buf[SHELL_DEFAULT_BUFSIZE];

    shell_run(shell_extended_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
