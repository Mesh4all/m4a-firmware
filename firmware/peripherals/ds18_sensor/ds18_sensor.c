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
 * @brief  Moisture sensor
 *
 * @author  xkevin190 <kevinvelasco190@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include "ds18.h"
#include "board.h"

ds18_t thermo;

int init_temperature_sensor (int pin)
{
    thermo.params.pin = GPIO_PIN(0, pin);
    thermo.params.out_mode = GPIO_OD_PU;

    // initialize the device
    int res = ds18_init(&thermo, &thermo.params);

    if (res == DS18_ERROR)
    {
        printf("DS18 not initialized\n");
        return DS18_ERROR;
    } else {
         printf("DS18 initialized\n");
    }

    return 0;
};

int get_temperature_value (int16_t* output)
{
    int16_t temp;

    int res = ds18_get_temperature(&thermo, &temp);

    if (res == DS18_OK)
    {
        *output = temp;
    } else {
        printf("Program halted, review configuration and try again\n");
        return DS18_ERROR;
    }

    return 0;
};
