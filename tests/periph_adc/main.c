/*
 * Copyright (c) 2022 Mesh4all mesh4all.org
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
 * @brief       ADC test
 *
 * @author      Luis A. Ruiz <luisan00@hotmail.com>
 */
#include <string.h>
#include <errno.h>
#include "periph/adc.h"
#include "ztimer.h"
#include "embUnit.h"

#define RES ADC_RES_10BIT
#define DELAY_MS 100U
#define LAPS 5

/* initialize all available ADC lines */
void test_adc_init(void) {
    int err;
    for (unsigned i = 0; i < ADC_NUMOF; i++) {
        if (adc_init(ADC_LINE(i)) < 0) {
            printf("\nInitialization of ADC_LINE(%u) failed\n", i);
            err = 1;
        } else {
            printf("\nSuccessfully initialized ADC_LINE(%u)\n", i);
            printf("Channel %u of %u\n", i + 1, ADC_NUMOF);
            err = 0;
        }
        ztimer_sleep(ZTIMER_MSEC, DELAY_MS);
    }

    TEST_ASSERT_EQUAL_INT(0, err);
}

void test_adc_read(void) {
    int err;
    int sample;
    for (int lap = 0; lap < LAPS; lap++) {

        printf("\nRound %d of %d\n", lap + 1, LAPS);

        for (unsigned i = 0; i < ADC_NUMOF; i++) {
            sample = adc_sample(ADC_LINE(i), RES);
            if (sample < 0) {
                printf("ADC_LINE(%u): selected resolution not applicable\n", i);
                err = 1;
            } else {
                printf("ADC_LINE(%u): %i\n", i, sample);
                err = 0;
            }
        }
        ztimer_sleep(ZTIMER_MSEC, DELAY_MS);
    }

    TEST_ASSERT_EQUAL_INT(0, err);
}

Test *test_periph_adc(void) {

    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(test_adc_init), // init. ADC lines available
        new_TestFixture(test_adc_read), // read value for each line
    };

    EMB_UNIT_TESTCALLER(test_periph_adc, NULL, NULL, fixtures);

    return (Test *)&test_periph_adc;
}
int main(void) {
    printf("Test ADC peripherals\n");
    TESTS_START();
    TESTS_RUN(test_periph_adc());
    TESTS_END();
    return 0;
}
