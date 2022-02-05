/*
 * Copyright (C) 2022 Mesh4all <mesh4all.org>
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
 *
 * @brief       Cbor encoder and decoder functions to manipulate a group of
 * data.
 * @author      xkevin190 <kevinvelasco190@gmail.com>
 * @author      eduazocar <eduardo@turpialdev.com>
 */

#include <stdio.h>

#include "cbor.h"
#include "esp_log.h"
#include "serialization.h"

esp_err_t encode(char *key, int64_t value, uint8_t *cbor_coded, size_t *len) {
    char buffer[100];
    CborEncoder root_encoder;
    CborError err = CborNoError;
    CborEncoder map_encoder;

    // Initialize the outermost cbor encoder and create the element_map
    cbor_encoder_init(&root_encoder, (uint8_t *)buffer, sizeof(buffer), 0);
    err = cbor_encoder_create_map(&root_encoder, &map_encoder, 1);

    if (err != CborNoError) {
        ESP_LOGE(__func__, "Error creating map\nReason:%s, Error No:%d", cbor_error_string(err),
                 err);
    }

    // Cbor pass to encode the string Key and the int64 value that belong it//
    cbor_encode_text_stringz(&map_encoder, key);
    cbor_encode_int(&map_encoder, value);

    cbor_encoder_close_container(&root_encoder, &map_encoder);
    *len = cbor_encoder_get_buffer_size(&root_encoder, (uint8_t *)buffer);

    memcpy(cbor_coded, buffer, *len);
    ESP_LOG_BUFFER_HEXDUMP(__func__, cbor_coded, *len, ESP_LOG_INFO);
    return ESP_OK;
}

esp_err_t encode_uint8(char *key, uint8_t *value, uint8_t *cbor_coded, size_t *len) {
    char buffer[100];
    CborEncoder root_encoder;
    CborError err = CborNoError;
    CborEncoder map_encoder;

    // Initialize the outermost cbor encoder and create the element_map
    cbor_encoder_init(&root_encoder, (uint8_t *)buffer, sizeof(buffer), 0);
    err = cbor_encoder_create_map(&root_encoder, &map_encoder, 1);

    if (err != CborNoError) {
        ESP_LOGE(__func__, "Error creating the map\nReason:%s, Error No:%d", cbor_error_string(err),
                 err);
    }

    // Cbor pass to encode the string Key and the int64 value that belong it//
    cbor_encode_text_stringz(&map_encoder, key);
    err = cbor_encode_byte_string(&map_encoder, value, sizeof(value) - 1);

    if (err != CborNoError) {
        ESP_LOGE(__func__, "Wrong passing string bytes\nReason:%s, Error No:%d",
                 cbor_error_string(err), err);
        return ESP_FAIL;
    }
    cbor_encoder_close_container(&root_encoder, &map_encoder);
    *len = cbor_encoder_get_buffer_size(&root_encoder, (uint8_t *)buffer);

    memcpy(cbor_coded, buffer, *len);
    ESP_LOG_BUFFER_HEXDUMP(__func__, cbor_coded, *len, ESP_LOG_INFO);
    return ESP_OK;
}

esp_err_t decode(uint8_t *buf, char *key, void *value, size_t len) {
    CborValue it;
    CborValue map_it;
    CborParser root_parser;
    CborError err;
    err = cbor_parser_init(buf, len, 0, &root_parser, &it);

    if (err != CborNoError) {
        ESP_LOGE(__func__, "error parser_init encoding\nReason:%s, Error No:%d",
                 cbor_error_string(err), err);
        return ESP_FAIL;
    }

    CborType type = cbor_value_get_type(&it);

    if (type == CborMapType) {
        err = cbor_value_map_find_value(&it, key, &map_it);

        if (err != CborNoError) {
            ESP_LOGE(__func__, "Error opening map\nReason:%s, Error No:%d", cbor_error_string(err),
                     err);
            return ESP_FAIL;
        }

        else if (cbor_value_is_valid(&map_it) && cbor_value_is_integer(&map_it)) {
            cbor_value_get_int64(&map_it, value);
            return ESP_OK;
        }

        else if (cbor_value_is_valid(&map_it) && cbor_value_is_byte_string(&map_it)) {
            uint8_t *val;
            size_t len_data;
            cbor_value_dup_byte_string(&map_it, &val, &len_data, &map_it);
            memcpy(value, val, len_data);
            free(val);
            return ESP_OK;
        }

        else {
            ESP_LOGE(__func__, "Data and owner not founded\nReason:%s, Error No:%d",
                     cbor_error_string(err), err);
            return ESP_FAIL;
        }
    }

    return ESP_OK;
}
