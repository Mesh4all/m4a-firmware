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
 *
 * @ingroup     wifi-subsys_components
 * @{
 * @brief       sntp (Simple Network Time Protocol)
 * @author      eduazocar <eduardo@turpialdev.com>
 *
 */

#ifndef PROTOCOL_SNTP_H
#define PROTOCOL_SNTP_H

#include <time.h>

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This is a main function to initialize the sntp server (This will not be taken, only have
 * to be applied inside of the function get_time_sntp())
 */
esp_err_t init_sntp(void);

/**
 * @brief this function sync with a server sntp to get a current time and date, this format is
 * returned in milliseconds (time posix)
 * @param [inout] curr_time* This is a pointer param to manage the time, this take a time_t var and
 * manage if was set in the system.
 * @param [inout] curr_timeinfo* pointer structure to detailed info of the time_t var, in this case
 * curr_time*
 * @param [in] time_format This is en input to specific the time format to use UTC
 */
esp_err_t get_time_sntp(time_t *curr_time, struct tm *curr_timeinfo, char *time_format);

#ifdef __cplusplus
}
#endif
#endif /* PROTOCOL_SNTP_H */
/** @} */
