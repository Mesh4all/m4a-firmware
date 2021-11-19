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
 * @author      luisan00 <luisan00@hotmail.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xtimer.h"
#include "epd_bw_spi.h"
#include "epd_bw_spi_params.h"
#include "epd_bw_spi_pictures.h"

const uint16_t riot_32_height = sizeof(riot_logo_32) / sizeof(riot_logo_32[0]);

/* Draw the large RIOT logo with a full refresh */
void draw_riot(epd_bw_spi_t *dev)
{
    epd_bw_spi_wake(dev);
    epd_bw_spi_init_full(dev);
    epd_bw_spi_activate(dev);
    epd_bw_spi_write_buffer(dev, (uint8_t *)riot_icon_200, sizeof riot_icon_200);
    epd_bw_spi_update_full(dev);
    epd_bw_spi_deactivate(dev);
    if (dev->params.rst_pin != GPIO_UNDEF)
    {
        epd_bw_spi_sleep(dev);
    }
}

/* Draw a small RIOT logo with a partial refresh */
void draw_small_riot(epd_bw_spi_t *dev, uint8_t x, uint16_t y)
{
    epd_bw_spi_wake(dev);
    epd_bw_spi_init_part(dev);
    epd_bw_spi_activate(dev);
    epd_bw_spi_set_area(dev, x, x + 32, y, y + riot_32_height);
    epd_bw_spi_write_buffer(dev, (uint8_t *)riot_logo_32, sizeof riot_logo_32);
    epd_bw_spi_update_part(dev);
    epd_bw_spi_deactivate(dev);
    if (dev->params.rst_pin != GPIO_UNDEF)
    {
        epd_bw_spi_sleep(dev);
    }
}

int main(void)
{
    epd_bw_spi_t dev;
    int init = epd_bw_spi_init(&dev, epd_bw_spi_params, epd_bw_spi_controller,
                               EPD_BW_SPI_DISPLAY_X, EPD_BW_SPI_DISPLAY_Y,
                               EPD_BW_SPI_Y_INC_X_INC);
    if (init != 0)
    {
        printf("EPD_BW_SPI INIT FAILED: %i\n", init);
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1)
    {
        /* Set both RAM buffers to the RIOT logo */
        draw_riot(&dev);
        draw_riot(&dev);
        xtimer_periodic_wakeup(&last_wakeup, 5 * US_PER_SEC);

        /* Draw small RIOT logos on the display */
        for (int y = 0; y < dev.size_y; y += riot_32_height)
        {
            for (int x = 0; x < dev.size_x; x += 32)
            {
                draw_small_riot(&dev, x, y);
                xtimer_periodic_wakeup(&last_wakeup, 2500 * US_PER_MS);
            }
        }
    }
    return 0;
}
