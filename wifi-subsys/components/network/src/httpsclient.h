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
 * @ingroup    wifi-subsys_components
 * @{
 * @file
 * @brief      Configure a https client to make a request
 * @author     RocioRojas <Rociorojas391@gmail.com>
 *
 */

#include "esp_err.h"
#include "esp_http_client.h"

#ifndef HTTPSCLIENT_H
#define HTTPSCLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 1024

/**
 * @brief Struct to save status, content_len, output.
 *
 */
typedef struct {
    int status;                               /*Status of the method */
    int content_len;                          /*size of output */
    uint8_t output[MAX_HTTP_OUTPUT_BUFFER];   /*Response buffer */
} http_response_t;

/**
 * @brief Type alias for response_callback_t
 *
 */
typedef void (*response_callback_t)(http_response_t*);

/**
 * @brief Struct to make http request.
 *
 */
typedef struct {
   int method;                    /*Make a method get/post*/
   char url[200];                 /*Url used*/
   char body[100];                /*body used with method post*/
   response_callback_t callback;
} http_request_t;

/**
 * @brief Event handler of request.
 *
 * @param evt [in]   HTTP Client events data.
 * @return esp_err_t ESP_OK: succeed, ESP_(others): fail
 */
esp_err_t _http_event_handler(esp_http_client_event_t *evt);

/**
 * @brief Makes https  get/post request
 * Note: To create certificate in console:
 * openssl s_client -showcerts -connect [URL]:443 </dev/null
 *
 * @param http_request [in]   Struct to make http request.
 */
void http_client(http_request_t* http_request);
#ifdef __cplusplus
}
#endif
#endif /* HTTPSCLIENT_H */
/** @} */