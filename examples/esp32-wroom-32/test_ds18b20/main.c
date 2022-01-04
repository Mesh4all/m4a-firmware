#include <stdio.h>
#include "xtimer.h"
#include "board.h"
#include "ds18.h"
#include "periph/gpio.h"

int16_t temp;
static ds18_t therm1;
static ds18_params_t therm1_params = {
    .pin = GPIO14,
    .out_mode = GPIO_OD_PU,
    .in_mode = GPIO_IN
};

int main(void)
{
    (ds18_init(&therm1, &therm1_params) == DS18_ERROR)? puts("Init failed") : puts("OK");
    while(1) {
        if(ds18_get_temperature(&therm1, &temp) == DS18_OK){
            printf("Tempertature: ");
            if (temp < 0) printf("-");
            printf("%d,%d °C\n",temp/100, temp%100);
        }
        else{
            puts("Could not read temperature");
        }
        if(temp > 2600){
            gpio_write(GPIO12, 1);
        }
        else{
            gpio_write(GPIO12, 0);
        }
        xtimer_sleep(1);
    } 
    return 0;
}
