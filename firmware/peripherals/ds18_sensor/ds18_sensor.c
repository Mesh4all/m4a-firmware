
#include <stdio.h>
#include <stdlib.h>
#include "ds18.h"
#include "board.h"

ds18_t thermo;
int error_count = 0;
int max_errors = 10;

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
