#include <stdio.h>
#include "xtimer.h"
#include "board.h"
#include "periph/gpio.h"
#include "periph/adc.h"

unsigned int data;
double volt;

int main (void) {
    (adc_init(5)<0)? puts("Error") : puts("We are ready");
     while (1) {
           data = adc_sample(5, ADC_RES_12BIT);
           volt = (data - 1370) * 3.3/ 4095;            // 1370 is the minimum value obtained from adc_sample
           volt = (3.3 - volt)*100 / 3.3;               // 4095 refers to the int value of the resolution
           printf("Humidity: %.2f",volt);
           puts(" %");
           xtimer_sleep(1);
    }

    return 0;

}
