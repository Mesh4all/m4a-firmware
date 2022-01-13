#include "serialization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbor.h"

int cbor_decode_message (uint8_t* buffer, sensor_data* data, size_t length)
{
    CborParser parser;
    CborValue it;
    CborValue moisture_in;
    CborValue temp_in;
    uint64_t temp;
    uint64_t moist;

    if (cbor_parser_init(buffer, length, 0, &parser, &it) != CborNoError) {
        printf("chat: couldn't parse chat cbor input");
        return -1;
    }

    if (!cbor_value_is_map(&it)) {
        printf("this is not map: \n");
        return -1;
    }

    if(cbor_value_map_find_value(&it, "moisture", &moisture_in) != CborNoError){
        printf("error in find moisture");
        return -1;
    };

    if (!cbor_value_is_valid(&moisture_in) &&
        !cbor_value_is_integer(&moisture_in)) {
        printf("fail moisture \n");
        return -1;
    }

    cbor_value_map_find_value(&it, "temp", &temp_in);

    if (!cbor_value_is_valid(&temp_in) &&
        !cbor_value_is_integer(&temp_in)) {
        printf("fail temp \n");
        return -1;
    }

    /* Copy byte string */
    cbor_value_get_uint64(&temp_in, &temp);
    cbor_value_get_uint64(&moisture_in, &moist);

    memcpy(&data->temperature, &temp, sizeof(int16_t));
    memcpy(&data->mositure, &moist, sizeof(int));

    return 0;
}

int cbor_enconde_message (sensor_data* data, uint8_t* output, size_t* len_output)
{
    uint8_t buffer[100];
    CborEncoder encoder;
    cbor_encoder_init(&encoder, buffer, sizeof(buffer), 0);

    CborEncoder map_encoder;
    cbor_encoder_create_map(&encoder, &map_encoder, 2);

    cbor_encode_text_stringz(&map_encoder, "moisture");
    cbor_encode_int(&map_encoder, data->mositure);

    cbor_encode_text_stringz(&map_encoder, "temp");
    cbor_encode_int(&map_encoder, data->temperature);

    cbor_encoder_close_container(&encoder, &map_encoder);

    size_t len = cbor_encoder_get_buffer_size(&encoder, buffer);

    memcpy(output, buffer, len);
    memcpy(len_output, &len, sizeof(size_t));

    return 0;
}
