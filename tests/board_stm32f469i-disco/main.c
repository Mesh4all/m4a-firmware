/*
 * Copyright (c) 2021 Mesh4all <mesh4all.org>
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
 * @author      luisan00 <luisan00@hotmail.com>
 *
 */
#include <stdio.h>
#include <string.h>

#include "xtimer.h"
#include "shell.h"
#include "shell_commands.h"
#include "msg.h"
#include "thread.h"
#include "log.h"

char *status_to_str(thread_status_t status)
{
    switch (status)
    {
    case 0:
        return "STATUS_STOPPED";
        break;
    case 3:
        return "STATUS_MUTEX_BLOCKED";
        break;

    default:
        return "STATUS_UNKNOW";
        break;
    }
}

char blink_thread[THREAD_STACKSIZE_MAIN];
kernel_pid_t blinker_pid = KERNEL_PID_UNDEF;

bool is_blinking = false;

void *blinker(void *arg)
{
    (void)arg;

    while (is_blinking)
    {
        LED0_ON;
        xtimer_msleep(500);
        LED0_OFF;
        xtimer_msleep(500);
    }

    return NULL;
}

int blink(int argc, char **argv)
{

    if (argc != 2)
    {
        puts("Usage:\n");
        puts("\tblink [start|stop]");
        return 1;
    }

    if (strcmp(argv[1], "start") == 0 && is_blinking == false)
    {
        is_blinking = true;
        blinker_pid = thread_create(blink_thread, sizeof(blink_thread),
                                    THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                    blinker, NULL, "blinker");
        printf("Blink status: %d\n", thread_get(blinker_pid)->status);
        return 0;
    }

    if (strcmp(argv[1], "stop") == 0 && is_blinking == true)
    {
        is_blinking = false;
        thread_get(blinker_pid)->pid = KERNEL_PID_UNDEF;
        printf("Blink status: %d\n", thread_get(blinker_pid)->status);
        return 0;
    }

    printf("blinking running with PID: %d\n", blinker_pid);
    printf("Blink status: %d\n", thread_get(blinker_pid)->status);
    printf("is_blinking: %d\n", is_blinking);
    return 1;
}

static shell_command_t shell_extended_commands[] = {
    {"blink", "dummy cmd descrption", blink},
    {NULL, NULL, NULL}};

int main(void)
{
    printf("~~ Welcome to Mesh4all ~~\n");
    /* Start shell */

    char line_buf[SHELL_DEFAULT_BUFSIZE];

    shell_run(shell_extended_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
