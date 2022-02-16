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
 * @ingroup     examples_esp32-wrover
 *
 * @file
 * @brief       this is an example of external_led_blink
 * @author      xkevin190 <kevinvelasco190@gmail.com>
 * @author      eduazocar <eduardo@turpialdev.com>
 */

#include <stdio.h>

#include "board.h"
#include "ili9341.h"
#include "ili9341_params.h"
#include "periph/gpio.h"
#include "shell.h"
#include "timex.h"
#include "xtimer.h"
#include "ztimer.h"

static char server_stack[THREAD_STACKSIZE_DEFAULT];

#define INTERVAL (1U * US_PER_SEC)

int dummy_cmd(int argc, char **argv) { return 0; }

static const shell_command_t shell_commands[] = {{"udp", "send udp packets", dummy_cmd},
                                                 {NULL, NULL, NULL}};

int main(void) {
    gpio_init(23, GPIO_OUT);
    int init = 0;
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        printf("executed");
        if (init == 0) {
            gpio_write(23, 1);
            init = 1;
        } else {
            gpio_write(23, 0);
            init = 0;
        }
    }
    printf("~~ Welcome this a test ~~\n");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
}
