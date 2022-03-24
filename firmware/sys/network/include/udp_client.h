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
 * @{
 * @file        udp_m4a.h
 * @brief       udp libs
 * @author      xkevin190 <kevinvelasco190@gmail.com.com>
 *
 */

#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief this function is used for send the sensors values
 *
 * @param port  [in]            port to which the message will be sent
 * @param address [int]         address to which the message will be sent
 * @param message [int]         in this parameter the sensor data will be sent
 * @param payload_len [int]     size of the message
 * @return int
 */
int udp_send(int *port, char *address, uint8_t *message, size_t *payload_len);

#ifdef __cplusplus
}
#endif
#endif /* UDP_CLIENT_H */
