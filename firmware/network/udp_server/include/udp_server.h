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
 * @ingroup     udp_server
 * @{
 * @file
 * @brief       udp server lib used to get the sensor values
 * @author      xkevin190 <kevinvelasco190@gmail.com.com>
 *
 */

#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief this is a struct used to init udp server
 */
typedef struct {
    char *port;                                     /*!< port where will listen udp server */
    void (*callback)(uint8_t *message, size_t len); /*!< this is a callback this function will
                                                    be executed when arrive new values */
} udpf_payload;

/**
 * @brief  udp server function this function is used for init udp server
 *
 * @param argv [in] this param will content the callback and port where will listen the server
 * @return  0 Satisfactory result
 *         -1 Failed result
 */
int udp_server(udpf_payload *argv);

/**
 * @brief This function function is used to set the callback function to the udp server thread
 * @param args [in] UDP payload pointer
 */
void *_udp_serverf(void *args);

#ifdef __cplusplus
}
#endif
#endif /* UDP_SERVER_H */
/** @} */
