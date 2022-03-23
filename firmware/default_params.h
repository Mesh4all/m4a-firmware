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
 * @ingroup     m4a-firmware
 * @{
 * @name    Defaults params for the m4a firmware
 * @author  xkevin190 <kevinvelasco193@gmail.com>
 */

#ifndef DEFAULT_PARAMS_H
#define DEFAULT_PARAMS_H

#include "storage.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name storage address this address will be the storage keys
 * @{
 */

/**
 * @brief first address of the page this name is optional must be change in the future
 */
#define ADDRESS1 (uint32_t) flashpage_addr(LAST_AVAILABLE_PAGE)

/**
 * @brief second address of the page this name is optional must be change in the future
 */
#define ADDRESS2 (uint32_t) flashpage_addr(LAST_AVAILABLE_PAGE - 1)

/**
 * @brief third address of the page this name is optional must be change in the future
 */
#define ADDRESS3 (uint32_t) flashpage_addr(LAST_AVAILABLE_PAGE - 2)
/** @} */

#ifdef __cplusplus
}
#endif
#endif /* DEFAULT_PARAMS_H */
/** @} */
