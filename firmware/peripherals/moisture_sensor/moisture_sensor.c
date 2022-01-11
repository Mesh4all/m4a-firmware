
#include <stdio.h>
#include <stdlib.h>
#include "periph/adc.h"

#define DEFAULT_MIN 356
#define DEFAULT_MAX 880
#define RES ADC_RES_10BIT

int init_moisture (void)
{
    if (adc_init(0) < 0) {
        printf("Initialization of ADC_LINE failed\n");
        return -1;
    } else {
        printf("Successfully initialized  moisture sensor \n");
        return 0;
    }
    return 0;
}

int get_moisture_value(int* output)
{
    int sample = adc_sample(0, RES);
    printf("ADC_LINE: SAMPLE %i \n", sample);
    if (sample < 0) {
         *output = -1;
        printf("ADC_LINE: selected resolution not applicable\n");
        return -1;
    } else if (sample == 0) {
        *output = -1;
        return -1;
    } else if (sample > 0) {
        int min = DEFAULT_MIN;
        int max = DEFAULT_MAX;
        int cal = max - min;
        double finished = ((sample - min) * 100 / cal);
        int moisture = 100 - finished;

        if(moisture <= 100 && moisture >= 0 ){
            // printf("the moisture  sensor is percent %i \n",  moisture);
            *output = moisture;
        } else if (moisture < 0) {
            // printf("the moisture sensor is %i percent \n",  0);
            *output = 0;
        } else if (moisture > 100) {
            // printf("the moisture sensor is %i percent \n",  100);
            *output = 100;
        }

        // here add uart_write to send the temperature since uart
    }

    return 0;
}
