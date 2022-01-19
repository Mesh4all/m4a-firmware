/*
 * Copyright (C) 2022 Mesh4all <contact@mesh4all.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 */

/**
 * @ingroup     slipdev
 *
 * @file
 * @{
 * @brief       A basic demo how works the slipdev
 *
 * @author      Eduardo Azócar <eduardo@turpialdev.com>
 *
 */
#include <stdio.h>
#include "xtimer.h"
#include "shell.h"
#include "shell_commands.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);
}
/**@}*/
