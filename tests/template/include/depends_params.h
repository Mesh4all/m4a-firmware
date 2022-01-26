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
 * @ingroup     test_template
 * @{
 * @file        depends_params.h
 * @brief       Template of depends_params, this a header only for some dependencies,
 * params and group of functions that coulb be used in the main code.
 *
 * @author      author_username <youremailaddr@email.com>
 *
 */

#ifndef DEPENDS_PARAMS_H
#define DEPENDS_PARAMS_H

#ifdef __cplusplus
extern "C"
{
#endif
/**
 * @brief  You could define some structs or data type inside your test
 *
 */
typedef struct {
   char* var1;  /*!< String example attribute*/
   int  var2;   /*!< Integer example attribute*/
} some_struct_var;

/**
 * @brief  Define your own functions that will be used in your tests.
 * @param param [in] This could be a param required to execute your function
 * @return  0 Satisfactory result
 *         -1 Failed result
 */
int test_function(int param);

#ifdef __cplusplus
}
#endif
#endif /* DEPENDS_PARAMS_H */
/** @} */
