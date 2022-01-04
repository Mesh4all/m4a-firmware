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
 * @file
 * @brief
 * @copyright Copyright (c) 2021
 */

#include "periph/adc.h"
#include "ztimer.h"

#define RES             ADC_RES_10BIT
#define DELAY_MS        5000U

int main (void) {

    if (adc_init(0) < 0) {
        printf("Initialization of ADC_LINE failed\n");
    } else {
        printf("Successfully initialized ADC_LINE\n");
    }
     while (1) {
           int sample = adc_sample(0, RES);
            if (sample < 0){
                printf("ADC_LINE: selected resolution not applicable\n");
            } else if (sample > 0) {
                int min = 356;
                int max = 901;
                int cal = max - min;
                double finished = ((sample - min) * 100 / cal) ;
                int moisture = 100 - finished;
                if(moisture <= 100 && moisture >= 0 ){
                    printf("the moisture  sensor is percent %i \n",  moisture);

                } else if (moisture < 0) {
                    printf("the moisture sensor is %i percent \n", 0);
                } else if (moisture > 100) {
                    printf("the moisture sensor is %i percent \n", 100);
                }
            }
        ztimer_sleep(ZTIMER_MSEC, DELAY_MS);
    }
    return 0;
}