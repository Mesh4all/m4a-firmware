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
/**
 * @ingroup storage
 * @{
 * @brief   Non Volatile Storage
 * @author  luisan00 <luisan00@hotmail.com>
 */
#ifndef STORAGE_H
#define STORAGE_H

#include <string.h>
#include <stdbool.h>
#include "esp_err.h"
#include "nvs_flash.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief initialize the default NVS partition
     *
     * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
     */
    esp_err_t nvs_init(void);

    /**
     * @brief store a key/value pair in the NVS using the given namespace
     *
     * @param [in] namespace
     * @param [in] key
     * @param [out] value
     * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
     */
    esp_err_t nvs_set_uint8(const char *namespace, const char *key, uint8_t value);

    /**
     * @brief Store a c string in the NVS using the given namespace and key
     *
     * @param [in] namespace
     * @param [in] key
     * @param [in] value
     * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
     */
    esp_err_t nvs_set_string(const char *namespace, const char *key, const char *value);

    /**
     * @brief Get the value as uint8_t with key [key] from the NVS using the given namespace
     *
     * @param [in] namespace
     * @param [in] key
     * @param [out] value
     * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
     */
    esp_err_t nvs_get_uint8(const char *namespace, const char *key, uint8_t *value);

    /**
     * @brief Get the value as c string with key [key] from the NVS using the given namespace
     *
     * @param [in] namespace
     * @param [in] key
     * @param [out] buffer
     * @param [out] length
     * @return esp_err_t
     */
    esp_err_t nvs_get_string(const char *namespace, const char *key, char *buffer, size_t *length);

#ifdef __cplusplus
}
#endif

#endif /* STORAGE_H */
/** @} */
