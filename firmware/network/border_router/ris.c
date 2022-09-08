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
#ifdef MODULE_GNRC_SIXLOWPAN_ND
#include "net/gnrc/sixlowpan/nd.h"
#endif
#include "net/ndp.h"

#include "net/gnrc/ndp.h"

#include "timex.h"
#include "ztimer.h"

#define ENABLE_DEBUG 1
#include "debug.h"

#define BRADV_PACKET_TYPE (210)
#define BORDER_ROUTER_MSG_QUEUE_SIZE (8)
static char _stack[THREAD_STACKSIZE_DEFAULT];
static msg_t _msg[BORDER_ROUTER_MSG_QUEUE_SIZE];
kernel_pid_t brr_pid = KERNEL_PID_UNDEF;
static gnrc_netreg_entry_t me_reg;

gnrc_pktsnip_t *radv_build_pkt(ipv6_addr_t located_route, uint8_t prefix, gnrc_pktsnip_t *next) {
    (void) next;
    assert(&located_route != NULL);
    assert(!ipv6_addr_is_link_local(&located_route) && !ipv6_addr_is_multicast(&located_route));
    assert(prefix <= 128);
    uint8_t wireless_iface = get_ieee802154_iface();
    gnrc_netif_t *wiface = gnrc_netif_get_by_pid(wireless_iface);
    gnrc_pktsnip_t *rpacket = NULL;
    br_msg_t msg;
    msg.ip = located_route;
    msg.prefix = prefix;
    gnrc_pktsnip_t *payload = gnrc_pktbuf_add(NULL, &msg, sizeof(br_msg_t), GNRC_NETTYPE_UNDEF);
    if (payload == NULL) {
        DEBUG("auto_subnets: unable to copy data to packet buffer\n");
    }
    rpacket =
        gnrc_icmpv6_build(payload, BRADV_PACKET_TYPE, 0, sizeof(icmpv6_hdr_t));
    gnrc_pktsnip_t *ip = gnrc_ipv6_hdr_build(rpacket, NULL, &ipv6_addr_all_nodes_link_local);
    if (ip == NULL) {
        DEBUG("auto_subnets: unable to allocate IPv6 header\n");
        gnrc_pktbuf_release(ip);
    }
    if (wiface != NULL) {
        gnrc_pktsnip_t *wiface_hdr = gnrc_netif_hdr_build(NULL, 0, NULL, 0);
        if (wiface == NULL) {
            DEBUG("auto_subnets: unable to allocate netif header\n");
            gnrc_pktbuf_release(ip);
        }

        gnrc_netif_hdr_set_netif(wiface_hdr->data, wiface);
        ip = gnrc_pkt_prepend(ip, wiface_hdr);
    }
    return ip;
}

void radv_pkt_send(void) {
    gnrc_pktsnip_t *ext_pkt = NULL;
    gnrc_pktsnip_t *pkt = NULL;
    uint8_t wire_idx = get_wired_iface();
    // uint8_t wireless_idx = get_ieee802154_iface();
    // gnrc_netif_t *radio_if = gnrc_netif_get_by_pid(wireless_idx);
    gnrc_ipv6_nib_ft_t rtable;
    void *state = NULL;
    if (!gnrc_ipv6_nib_ft_iter(NULL, wire_idx, &state, &rtable)) {
        return;
    }
    pkt = radv_build_pkt(rtable.dst, rtable.dst_len, ext_pkt);
    if (pkt != NULL) {
        if (!gnrc_netapi_dispatch_send(GNRC_NETTYPE_NDP, GNRC_NETREG_DEMUX_CTX_ALL, pkt)) {
            DEBUG("ndp: unable to send router solicitation\n");
            return;
        }
    } else {
        DEBUG("Router Advertisement Failed\n");
    }
}

// gnrc_pktsnip_t *rsol_build_pkt(gnrc_pktsnip_t *next) {
//     assert(located_route != NULL);
//     assert(!ipv6_addr_is_link_local(located_route) && !ipv6_addr_is_multicast(located_route));
//     assert(prefix <= 128);
//     gnrc_pktsnip_t *pkt = gnrc_ndp_opt_ri_build(located_route, prefix,
//     NDP_OPT_PI_VALID_LTIME_INF,
//                                                 NDP_OPT_RI_FLAGS_PRF_ZERO, next);
//     if (pkt == NULL) {
//         DEBUG("ndp: NA not created due to no space in packet buffer\n");
//     }
//     return pkt;
// }

// void rsol_pkt_send(void){

// }

static void *_event_loop(void *args) {
    (void)args;
    msg_t msg;
    msg_init_queue(_msg, BORDER_ROUTER_MSG_QUEUE_SIZE);
    uint16_t period = 3;
    uint8_t times = 0;
    // uint8_t wired_iface = get_wired_iface();
    // ipv6_addr_t addr;
    /* start event loop */
    while (1) {
        DEBUG("RPL: waiting for incoming message.\n");
        if ((ztimer_msg_receive_timeout(ZTIMER_SEC, &msg, (uint32_t)period) == -ETIME)) {
            radv_pkt_send();
            times++;
            if (times > 2) {
                if (period < 180) {
                    period *= 180;
                } else if (period >= 180) {
                    period /= 180;
                }
                times = 0;
            }
        } else {
            switch (msg.type) {
            case GNRC_NETAPI_MSG_TYPE_RCV:
                DEBUG("RTR advertisement receives\n");
                ztimer_sleep(ZTIMER_USEC, 200);
                radv_pkt_send();
                break;
            default:
                break;
            }
        }
    }
    return NULL;
}

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
