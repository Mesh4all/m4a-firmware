/*
 * Copyright (C) 2016 Kees Bakker, SODAQ
 *               2018 Freie Universität Berlin
 * Copyright (C) 2022 Mesh4all mesh4all.org
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
/**
 * @ingroup     test_group_driver_bme280
 * @{
 *
 * @file
 * @brief       Test application for the BME280 temperature, pressure, and
 *              humidity sensor driver
 *
 * @author      Kees Bakker <kees@sodaq.com>
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 * @author      Luis A. Ruiz <luisan00@hotmail.com>
 * @}
 */

#include <stdlib.h>
#include <stdio.h>

#include "bmx280_params.h"
#include "bmx280.h"
#include "fmt.h"
#include "embUnit.h"

bmx280_t bme280;
#if (MODULE_BME280_I2C) || (MODULE_BME_280_SPI)
char sensor_id = 'E';
#else
char sensor_id = 'P';
#endif

static const bmx280_params_t default_bme280_params[] = {
    {.i2c_dev = I2C_DEV(0), .i2c_addr = 0x76, BMX280_PARAM_MISC},
};

void bmx280_init_test(void) {
    // It should be initialized
    int error = bmx280_init(&bme280, &default_bme280_params[0]);

    switch (error) {
    case BMX280_ERR_BUS:
        printf("[Error] Something went wrong when using the I2C bus\n");
        break;
    case BMX280_ERR_NODEV:
        printf("[Error] Unable to communicate with the BM%c280 device\n", sensor_id);
        break;
    case BMX280_OK:
        printf("BM%c280 - Initialized\n", sensor_id);
        break;
    default:
        printf("[Error] This should never happen\n");
        break;
    }
    TEST_ASSERT_EQUAL_INT(0, error);
}

void bmx280_get_temperature(void) {
    // It should be able to read temperature
    int16_t temperature = bmx280_read_temperature(&bme280);

    char str_temperature[8];
    size_t len = fmt_s16_dfp(str_temperature, temperature, -2);
    str_temperature[len] = '\0';
    printf("BM%c280 - Temperature: %s ºC\n", sensor_id, str_temperature);

    // It should be greater than -32768
    TEST_ASSERT(temperature > -32768);
}

void bmx280_get_pressure(void) {
    // It should be able to read pressure in Pascal [Pa]
    uint32_t pressure = bmx280_read_pressure(&bme280);

    printf("BM%c280 - Pressure: %" PRIu32 " Pa\n", sensor_id, pressure);

    // It should be less than 4294967295 or 0xFFFFFFFF
    TEST_ASSERT(pressure < 4294967295);
}

#if (MODULE_BME280_I2C) || (MODULE_BME_280_SPI)
void bme280_get_humiditty(void) {
    // It should be able to read humidity
    uint16_t humidity = bme280_read_humidity(&bme280);

    char str_humidity[8];
    size_t len = fmt_s16_dfp(str_humidity, humidity, -2);
    str_humidity[len] = '\0';
    printf("BME280 - Humidity: %s %%rH\n", str_humidity);

    // It should be greater than zero
    TEST_ASSERT(humidity > 0);
}
#endif

Test *test_bme280_driver(void) {

    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(bmx280_init_test),
        new_TestFixture(bmx280_get_temperature),
        new_TestFixture(bmx280_get_pressure),
#if (MODULE_BME280_I2C) || (MODULE_BME_280_SPI)
        new_TestFixture(bme280_get_humiditty),
#endif
    };

    EMB_UNIT_TESTCALLER(test_bme280_driver, NULL, NULL, fixtures);

    return (Test *)&test_bme280_driver;
}

int main(void) {
    printf("\n");
    printf("Test BM%c280 driver\n", sensor_id);

    TESTS_START();
    TESTS_RUN(test_bme280_driver());
    TESTS_END();

    return 0;
}
