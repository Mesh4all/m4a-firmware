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
 * @{
 * @file        udpf.h
 * @brief       Main firmware file
 *
 * @copyright   Copyright (c) 2021 Mesh for all
 * @author      luisan00 <luisan00@hotmail.com>
 *
 */

#ifndef UDPF_H
#define UDPF_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
   char* port;
   void (*callback) (char*);
} udpf_payload;

int udp_server(int argc, udpf_payload* argv);
int udp_send(int argc, char **argv);

#ifdef __cplusplus
}
#endif
#endif /* UDPF_H */
/** @} */
