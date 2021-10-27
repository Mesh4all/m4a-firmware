#include <stdio.h>
#include <inttypes.h>

#include "board.h"
#include "ds18.h"
#include "xtimer.h"

int main(void)
{
    int error_count = 0;
    int max_errors = 3;

    ds18_t thermo;
    thermo.params.pin = GPIO_PIN(PA, 20);
    thermo.params.out_mode = GPIO_OD_PU;

    // initialize the device
    int res = ds18_init(&thermo, &thermo.params);

    if (res == DS18_ERROR)
    {
        printf("DS18 not initialized\n");
        return DS18_ERROR;
    }
    while (1)
    {
        int16_t temp;

        res = ds18_get_temperature(&thermo, &temp);

        if (res == DS18_OK)
        {
            printf("Temp: %c%d.%02d ºC\n", (temp < 0) ? '-' : ' ', temp / 100, temp % 100);
        }
        else
        {
            error_count += 1;
            printf("ERROR %d/%d: Can't read temperature in the given device\n", error_count, max_errors);
            if (error_count == max_errors)
            {
                printf("Program halted, review configuration and try again\n");
                return DS18_ERROR;
            }
        }
        /*
        if (temp < 0)
        {
            temp = -temp;
        }
    */

        xtimer_sleep(3);
    }
}