/*
 * Copyright (C) 2021 Mesh4all <mesh4all.org>
 *
 * This file is is licensed under the Apache License, Version 2.0,
 * you may not use this file except in compliance with the License.
 * See the LICENSE file in the root folder for more information
 */

/**
 * @author      luisan00 <luisan00@hotmail.com>
 */
#include <stdio.h>

#include "xtimer.h"
#include "shell.h"
#include "shell_commands.h"
#include "msg.h"
#include "log.h"

int dummy_cmd(int argc, char **argv){
    LED0_TOGGLE;
    xtimer_msleep(1000);
    LED0_TOGGLE;
    return 0;

}

static shell_command_t shell_extended_commands[] = {
    {"dummy_cmd", "dummy cmd descrption", dummy_cmd},
    {NULL, NULL, NULL}};

int main(void)
{
    printf("~~ Welcome to Mesh4all ~~\n");
    /* Start shell */
    
    char line_buf[SHELL_DEFAULT_BUFSIZE];

    shell_run(shell_extended_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
