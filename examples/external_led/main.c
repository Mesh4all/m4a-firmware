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
#include "periph/gpio.h"
#include "xtimer.h"

#define INTERVAL (1U * US_PER_SEC)

int main(void) {
    gpio_init(LED0_PIN, GPIO_OUT);
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        gpio_toggle(LED0_PIN);
    }
}
