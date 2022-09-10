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
 * @brief   CHAMOS  Communication Handler for Addressing Management | Origin Server
 *
 * @author  RocioRojas <Rociorojas391@gmail.com>
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "border_router.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/icmpv6.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/netif/internal.h"
#include "net_tools.h"
#include "radio.h"
#include "random.h"
#ifdef MODULE_GNRC_SIXLOWPAN_ND
#include "net/gnrc/sixlowpan/nd.h"
#endif
#include "net/ndp.h"

#include "net/gnrc/ndp.h"

#include "timex.h"
#include "ztimer.h"

#define ENABLE_DEBUG 0
#include "debug.h"

#define BORDER_ROUTER_MSG_QUEUE_SIZE (8)
static char _stack[THREAD_STACKSIZE_DEFAULT];
static msg_t _msg[BORDER_ROUTER_MSG_QUEUE_SIZE];
kernel_pid_t brr_pid = KERNEL_PID_UNDEF;
static gnrc_netreg_entry_t me_reg;

// static int8_t check_routes_wired_if(void);

gnrc_pktsnip_t *radv_build_pkt(ipv6_addr_t located_route, uint8_t prefix, gnrc_pktsnip_t *next) {
    assert(&located_route != NULL);
    assert(!ipv6_addr_is_link_local(&located_route) && !ipv6_addr_is_multicast(&located_route));
    assert(prefix <= 128);
    gnrc_pktsnip_t *pkt = gnrc_ndp_opt_ri_build(&located_route, prefix, 300,
                                                NDP_OPT_RI_FLAGS_PRF_ZERO, next);
    if (pkt == NULL) {
        DEBUG("ndp: NA not created due to no space in packet buffer\n");
        gnrc_pktbuf_release(pkt);
    }
    return pkt;
}

void radv_pkt_send(void) {
    gnrc_pktsnip_t *ext_pkt = NULL;
    gnrc_pktsnip_t *pkt = NULL;
    // uint8_t wire_idx = get_wired_iface();
    uint8_t wireless_idx = get_ieee802154_iface();
    gnrc_netif_t *radio_if = gnrc_netif_get_by_pid(wireless_idx);
    gnrc_ipv6_nib_ft_t rtable;
    void *state = NULL;
    while(gnrc_ipv6_nib_ft_iter(NULL, 0, &state, &rtable)) {
        pkt = radv_build_pkt(rtable.dst, rtable.dst_len, ext_pkt);
        ext_pkt = pkt;
    }
    
    if (pkt != NULL) {
        gnrc_ndp_rtr_adv_send(radio_if, NULL, &ipv6_addr_all_routers_link_local, true, pkt);
    } else {
        DEBUG("Router Advertisement Failed\n");
    }
}

static void *_event_loop(void *args) {
    (void)args;
    msg_t msg;
    msg_init_queue(_msg, BORDER_ROUTER_MSG_QUEUE_SIZE);
    uint32_t period = 3;
    uint8_t times = 0;
    bool rand_send = true;
    /* start event loop */
    // uint8_t wireless_idx = get_ieee802154_iface();
    // gnrc_netif_t *wless_iface = gnrc_netif_get_by_pid(wireless_idx);
    uint8_t wire_idx = get_wired_iface();
    while (1) {
        DEBUG("RPL: waiting for incoming message.\n");

        if ((ztimer_msg_receive_timeout(ZTIMER_SEC, &msg, rand_send? random_uint32() % period : period) == -ETIME)) {
            radv_pkt_send();
            times++;
            if (times > 3) {
                if (period < 30) {
                    period *= 10;
                    rand_send = false;
                }
                else if (period >= 30) {
                    period /= 10;
                    rand_send = true;
                }
                times = 0;
            }
        } else {
            switch (msg.type) {
            case GNRC_NETAPI_MSG_TYPE_RCV:
                DEBUG("RTR advertisement receives\n");
                gnrc_ipv6_nib_change_rtr_adv_iface(gnrc_netif_get_by_pid(wire_idx), false);
                break;
            default:
                break;
            }
        }
    }
    return NULL;
}

// static int8_t check_routes_wired_if(void) {
//     gnrc_ipv6_nib_ft_t rtable;
//     void *state = NULL;
//     uint8_t wire_idx = get_wired_iface();
//     if (!gnrc_ipv6_nib_ft_iter(NULL, wire_idx, &state, &rtable)) {
//         return 0;
//     }
//     // if (memcmp(&rtable.dst, &ipv6_addr_unspecified, sizeof(ipv6_addr_t))){
//     //     return -1;
//     // }
//     return -1;
// }

int8_t init_br_routing(void) {
    if (brr_pid != KERNEL_PID_UNDEF) {
        DEBUG("Border Router Routing Protocol is active\n");
        return -1;
    }
    brr_pid = thread_create(_stack, sizeof(_stack), 6, THREAD_CREATE_STACKTEST, _event_loop, NULL,
                            "routing_br");
    me_reg.demux_ctx = ICMPV6_RTR_ADV;
    me_reg.target.pid = brr_pid;
    gnrc_netreg_register(GNRC_NETTYPE_ICMPV6, &me_reg);
    return 0;
}
