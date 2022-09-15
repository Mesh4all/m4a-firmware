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
 * @brief  serialization implementation for this it will be used tinycbor
 *
 * @author  xkevin190 <kevinvelasco190@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serialization.h"
#include "cbor.h"
#include "board.h"

#if (CONFIG_DEBUG_SERIALIZATION) || (DOXYGEN)
/**
 * @brief KCONFIG_PARAMETER TO SET DEBUG MODE
 *
 */
#define ENABLE_DEBUG CONFIG_DEBUG_SERIALIZATION
#else
#define ENABLE_DEBUG 0
#endif
#include "debug.h"

int cbor_decode_message(uint8_t *buffer, sensor_data *data, size_t length) {
    CborParser parser;
    CborValue it;
    CborValue moisture_in;
    CborValue temp_in;
    CborValue nodeid_in;
    uint64_t temp;
    uint64_t moist;

    if (cbor_parser_init(buffer, length, 0, &parser, &it) != CborNoError) {
        DEBUG("chat: couldn't parse chat cbor input");
        return -1;
    }

    if (!cbor_value_is_map(&it)) {
        DEBUG("this is not map: \n");
        return -1;
    }

    if (cbor_value_map_find_value(&it, "moisture", &moisture_in) != CborNoError) {
        DEBUG("error in find moisture");
        return -1;
    }

    if (!cbor_value_is_valid(&moisture_in) && !cbor_value_is_integer(&moisture_in)) {
        DEBUG("fail moisture \n");
        return -1;
    }

    ///////////////////////

    if (cbor_value_map_find_value(&it, "node_id", &nodeid_in) != CborNoError) {
        DEBUG("error in find id");
        return -1;
    }

    if (!cbor_value_is_byte_string(&nodeid_in)) {
        DEBUG("fail id2 \n");
        return -1;
    }

    size_t len = 0;
    cbor_value_calculate_string_length(&nodeid_in, &len);
    if (len == 0) {
        return -1;
    }

    ///////////////////

    cbor_value_map_find_value(&it, "temp", &temp_in);

    if (!cbor_value_is_valid(&temp_in) && !cbor_value_is_integer(&temp_in)) {
        DEBUG("fail temp \n");
        return -1;
    }

    cbor_value_get_uint64(&temp_in, &temp); // Copy byte string
    cbor_value_get_uint64(&moisture_in, &moist);
    cbor_value_copy_byte_string(&nodeid_in, (uint8_t *)data->id_node, &len, NULL);

    memcpy(&data->temperature, &temp, sizeof(int16_t));
    memcpy(&data->soil_moisture, &moist, sizeof(int));
    return 0;
}

int cbor_enconde_message(sensor_data *data, uint8_t *output, size_t *len_output) {
    uint8_t buffer[CONFIG_SERIALIZATION_BUFF_SIZE];
    CborEncoder encoder;
    cbor_encoder_init(&encoder, buffer, sizeof(buffer), 0);

    CborEncoder map_encoder;
    cbor_encoder_create_map(&encoder, &map_encoder, 3);

    cbor_encode_text_stringz(&map_encoder, "moisture");
    cbor_encode_int(&map_encoder, data->soil_moisture);

    cbor_encode_text_stringz(&map_encoder, "temp");
    cbor_encode_int(&map_encoder, data->temperature);

    cbor_encode_text_stringz(&map_encoder, "node_id");
    int ret = cbor_encode_byte_string(&map_encoder, (uint8_t *)data->id_node, CPUID_LEN);
    if (ret != CborNoError) {
        DEBUG("cbor error %d", ret);
    }

    cbor_encoder_close_container(&encoder, &map_encoder);

    size_t len = cbor_encoder_get_buffer_size(&encoder, buffer);

    memcpy(output, buffer, len);
    memcpy(len_output, &len, sizeof(size_t));

    return 0;
}
