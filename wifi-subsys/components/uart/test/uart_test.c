/*
 * Copyright (c) 2022 Mesh4all <mesh4all.org>
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

#include "unity.h"
#include "subsys_uart.h"
#include "esp_err.h"

TEST_CASE("uart initialize", "[UART]")
{
    esp_err_t err;

    err = init_uart();

    if (err != ESP_OK) TEST_FAIL();
}

TEST_CASE("send uart bytes", "[UART]")
{
    const char* test = "test_data";
    int byteslen = sendData("uart_test", test);

    if(byteslen == 0) TEST_FAIL();
}
