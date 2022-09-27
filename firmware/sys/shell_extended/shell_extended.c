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
 * @brief   Extends functionality on the shell for uniqueid
 * @author  Luis A. Ruiz    <luisan00@hotmail.com>
 * @author  Eduardo Azócar  <eduazocarv@gmail.com>
 */

#include "shell_extended.h"
#include "kernel_defines.h"

#if IS_USED(MODULE_UNIQUEID)
int uid_cmd(int argc, char **argv);
#endif
#if IS_USED(MODULE_STORAGE)
int storage_cmd(int argc, char **argv);
#endif

const shell_command_t shell_extended_commands[] = {
#if IS_USED(MODULE_UNIQUEID)
    {"uid", "uniqueid commands", uid_cmd},
#endif
#if IS_USED(MODULE_STORAGE)
    {"stg", "Storage commands", storage_cmd},
#endif
    {NULL, NULL, NULL},
};
