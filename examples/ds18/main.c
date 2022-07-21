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
 * @ingroup     examples_ds18
 *
 * @brief       This is a example to understand how tu works the ds18b20
 *
 * @author      Eduardo Azócar <eduardo@turpialdev.com>
 *
 */

#include <stdio.h>
#include "xtimer.h"
#include "ds18_sensor.h"
#include "periph/gpio.h"

int main(void) {
    int16_t temp = 0;
    init_temperature_sensor(5) ? printf("DS18_Sensor_initialized\n")
                               : printf("DS18_Sensor wrong initialization\n");
    while (1) {
        get_temperature_value(&temp);
        printf("Temperature: ");
        if ((temp < 0) && (temp > -100)) {
            printf("-");
        }
        printf("%d,%d °C\n", temp / 100, (uint16_t)temp % 100);
        if (temp > 2600) {
            gpio_write(LED0_PIN, 0);
        } else {
            gpio_write(LED0_PIN, 1);
        }
        xtimer_sleep(1);
    }
    return 0;
}
