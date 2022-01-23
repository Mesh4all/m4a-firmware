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
 * @brief   Non Volatile Storage
 *
 * @author  luisan00 <luisan00@hotmail.com>
 */

#include "esp_log.h"
#include "storage.h"

static const char *TAG = "NVS";

esp_err_t nvs_init(void)
{
    esp_err_t err = nvs_flash_init();

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Fail %s, Initializing the default NVS partition", esp_err_to_name(err));
        return err;
    }

    ESP_LOGD(TAG, "Succeed ESP_OK");

    return ESP_OK;
}

esp_err_t nvs_set_uint8(const char *namespace, const char *key, uint8_t value)
{

    nvs_handle_t handle;

    esp_err_t err = nvs_open(namespace, NVS_READWRITE, &handle);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error: %s, trying to open the namespace %s", esp_err_to_name(err), namespace);
        return err;
    }

    err = nvs_set_u8(handle, key, value);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error: %s, setting the key: %s with value: %d", esp_err_to_name(err), key, value);
        return err;
    }

    err = nvs_commit(handle);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error: %s, committing changes", esp_err_to_name(err));
    }

    nvs_close(handle);
    return ESP_OK;
}

esp_err_t nvs_set_string(const char *namespace, const char *key, const char *value)
{

    nvs_handle_t handle;

    esp_err_t err = nvs_open(namespace, NVS_READWRITE, &handle);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %s: trying to open the namespace %s", esp_err_to_name(err), namespace);
        return err;
    }

    err = nvs_set_str(handle, key, value);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error: %s, setting the key: %s with value: %s", esp_err_to_name(err), key, value);
        return err;
    }

    err = nvs_commit(handle);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error: %s, committing changes", esp_err_to_name(err));
    }

    nvs_close(handle);
    return ESP_OK;
}

esp_err_t nvs_get_uint8(const char *namespace, const char *key, uint8_t *value)
{
    nvs_handle_t handle;

    esp_err_t err = nvs_open(namespace, NVS_READONLY, &handle);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %s: trying to open the namespace %s", esp_err_to_name(err), namespace);
        return err;
    }

    err = nvs_get_u8(handle, key, value);

    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGD(TAG, "Error %s: not found", esp_err_to_name(err));
        return err;
    }
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %s: getting the vañue for key %s", esp_err_to_name(err), key);
    }

    nvs_close(handle);

    return ESP_OK;
}

esp_err_t nvs_get_string(const char *namespace, const char *key, char *buffer, size_t *length)
{
    nvs_handle_t handle;

    esp_err_t err = nvs_open(namespace, NVS_READONLY, &handle);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %s: trying to open the namespace %s", esp_err_to_name(err), namespace);
        return err;
    }

    err = nvs_get_str(handle, key, NULL, length);

    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGE(TAG, "Error %s: not found", esp_err_to_name(err));

        return err;
    }

    err = nvs_get_str(handle, key, buffer, length);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %s: getting the vañue for key %s", esp_err_to_name(err), key);
    }

    nvs_close(handle);

    return ESP_OK;
}
