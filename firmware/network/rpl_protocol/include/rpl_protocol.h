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
 * @ingroup rpl_protocol
 * @{
 *
 * @brief       this module content all functions of rpl
 * @author      xkevin190 <kevinvelasco193@gmail.com.com>
 *
 */

#ifndef RPL_PROTOCOL_H
#define RPL_PROTOCOL_H

#include "net/ipv6/addr.h"
#include "sched.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Configuration to set "DAG" or "DODAG" mode
 *
 */
#ifndef CONFIG_IS_DODAG
#define CONFIG_IS_DODAG (0)
#endif

/**
 * @brief DODAG instance by Kconfig
 *
 */
#ifndef CONFIG_DODAG_INSTANCE
#define CONFIG_DODAG_INSTANCE (1)
#endif

/**
 * @enum rpl_modes_t these are two acceptable modes DAG or DODAG
 */
enum rpl_modes_t {
    DAG = 0, /*!<Value of DAG mode in RPL*/
    DODAG    /*!<Value of DODAG mode in RPL*/
};

/**
 * @brief this function init rpl protocol
 *
 * @param iface_pid [in] interface where is the rpl protocol
 * @return uint8_t
 */
int8_t rpl_init(kernel_pid_t iface_pid);

/**
 * @brief init the node like a dodag(master node)
 *
 *
 * @param dodag_instance [in] dodag instance
 * @param root_address   [in]  dodag address
 * @return uint8_t
 */
int8_t gnrc_rpl_dodag_root(uint8_t dodag_instance, ipv6_addr_t *root_address);

/**
 * @brief this function remove the dodag instance
 *
 * @param instance_id
 * @return uint8_t
 */
int8_t rpl_dodag_remove(uint8_t instance_id);

/**
 * @brief  this function executed all it need for the correct work of rpl
 * @param  mode Only exists two possible cases to mode DAG(0) or DODAG(1)
 *
 * @return int8_t
 */
int8_t rpl_setup(uint8_t mode);

#ifdef __cplusplus
}
#endif
#endif /* RPL_PROTOCOL_H */
/**@}*/
