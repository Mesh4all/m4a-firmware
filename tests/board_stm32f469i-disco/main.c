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

int main(void)
{
    printf("~~ Welcome to Mesh4all ~~\n");
    while (1)
    {
        xtimer_msleep(1000);
        LED0_OFF;
        LED3_OFF;
        xtimer_msleep(50);
        LED0_ON;
        xtimer_msleep(100);
        LED3_ON;
        xtimer_msleep(1000);
        LED1_OFF;
        LED2_OFF;
        xtimer_msleep(50);
        LED1_ON;
        xtimer_msleep(100);
        LED2_ON;
    }

    return 0;
}
