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

#define ENABLE_DEBUG 1
#include "debug.h"

#define BORDER_ROUTER_MSG_QUEUE_SIZE    (8)
static char _stack[THREAD_STACKSIZE_DEFAULT];
static msg_t _msg[BORDER_ROUTER_MSG_QUEUE_SIZE];
kernel_pid_t brr_pid = KERNEL_PID_UNDEF;
static gnrc_netreg_entry_t me_reg;

void radv_pkt_send(uint8_t iface_idx, gnrc_pktsnip_t *ext_pkt);
// static kernel_pid_t routing;

// static void br_routing() {
//     gnrc_netif_t *downstream = NULL;
//     gnrc_pktsnip_t *ext_pkt = NULL;
//     const ipv6_addr_t *prefix = &pio->prefix;
//     uint32_t valid_ltime = byteorder_ntohl(pio->valid_ltime);
//     uint32_t pref_ltime = byteorder_ntohl(pio->pref_ltime);
//     const uint8_t prefix_len = pio->prefix_len;
//     uint8_t new_prefix_len, subnet_len;
// }

// static gnrc_pktsnip_t *_build_headers(gnrc_netif_t *netif,
//                                       const ipv6_addr_t *src,
//                                       const ipv6_addr_t *dst,
//                                       gnrc_pktsnip_t *payload)
// {
//     gnrc_pktsnip_t *l2hdr;
//     gnrc_pktsnip_t *iphdr = gnrc_ipv6_hdr_build(payload, src, dst);

//     if (iphdr == NULL) {
//         DEBUG("ndp: error allocating IPv6 header.\n");
//         return NULL;
//     }
//     ((ipv6_hdr_t *)iphdr->data)->hl = NDP_HOP_LIMIT;
//     /* add netif header for send interface specification */
//     l2hdr = gnrc_netif_hdr_build(NULL, 0, NULL, 0);
//     if (l2hdr == NULL) {
//         DEBUG("ndp: error allocating netif header.\n");
//         gnrc_pktbuf_remove_snip(iphdr, iphdr);
//         return NULL;
//     }
//     gnrc_netif_hdr_set_netif(l2hdr->data, netif);
//     return gnrc_pkt_prepend(iphdr, l2hdr);
// }

gnrc_pktsnip_t *radv_buld_pkt(ipv6_addr_t *located_route, uint8_t prefix, gnrc_pktsnip_t *next) {
    assert(located_route != NULL);
    assert(!ipv6_addr_is_link_local(located_route) && !ipv6_addr_is_multicast(located_route));
    assert(prefix <= 128);
    // uint8_t adv_flags = 0;
    gnrc_pktsnip_t *pkt =  gnrc_ndp_opt_ri_build(located_route, prefix, NDP_OPT_PI_VALID_LTIME_INF,
                                    NDP_OPT_RI_FLAGS_PRF_ZERO, next);
    // else {
    //     DEBUG("ndp: NA not created due to no space in packet buffer\n");
    // }
    return pkt;
}

void radv_pkt_send(uint8_t iface_idx, gnrc_pktsnip_t *ext_pkt) {
    gnrc_pktsnip_t *pkt = NULL;
    uint8_t wire_idx = get_wired_iface();
    gnrc_netif_t *radio_if = gnrc_netif_get_by_pid(iface_idx);
    gnrc_ipv6_nib_ft_t rtable;
    void *state = NULL;
    gnrc_ipv6_nib_ft_iter(NULL, wire_idx, &state, &rtable);
    pkt = radv_buld_pkt(&rtable.dst, rtable.dst_len, ext_pkt);
    if (pkt) {
        gnrc_ndp_rtr_adv_send(radio_if, NULL, NULL, true, pkt);
    } else {
        DEBUG("auto_subnets: Options empty, not sending RA\n");
    }
}

static void _receive(gnrc_pktsnip_t *pkt){
    gnrc_pktsnip_t *ipv6 = NULL;
    br_radv_t *radv_msg = pkt->data;
    ipv6_hdr_t *ipv6_hdr;
    void *state = NULL;
    gnrc_ipv6_nib_ft_t rtable;
    ipv6 = gnrc_pktsnip_search_type(pkt, GNRC_NETTYPE_IPV6);
    assert(ipv6 != NULL);
    ipv6_hdr = (ipv6_hdr_t *)ipv6->data;
    uint8_t wireless_idx = get_ieee802154_iface();
    while(gnrc_ipv6_nib_ft_iter(NULL, wireless_idx, &state, &rtable)){
        if((memcmp(&rtable.dst, &radv_msg->dest_route, sizeof(ipv6_addr_t)) == 0) && (rtable.dst_len == radv_msg->prefix)){
            DEBUG("Route already exist\n");
            return;
        }
    }
    gnrc_ipv6_nib_ft_add(&radv_msg->dest_route, radv_msg->prefix, &ipv6_hdr->src, wireless_idx, 0);
}

static void *_event_loop(void *args)
{
    (void)args;
    msg_t msg;
    msg_init_queue(_msg, BORDER_ROUTER_MSG_QUEUE_SIZE);
    /* start event loop */
    while (1) {
        DEBUG("RPL: waiting for incoming message.\n");
        msg_receive(&msg);
        switch (msg.type) {
            case GNRC_NETAPI_MSG_TYPE_RCV:
                DEBUG("RTR advertisement receives\n");
                _receive(msg.content.ptr);
                break;
            default:
                break;
        }
    }
    return NULL;
}



int8_t init_br_routing(void){
    if(brr_pid != KERNEL_PID_UNDEF){
        DEBUG("Border Router Routing Protocol is active\n");
        return -1;
    }
    brr_pid = thread_create(_stack, sizeof(_stack), 6,
                                     THREAD_CREATE_STACKTEST,
                                     _event_loop, NULL,
                                     "routing_br");
    me_reg.demux_ctx = ICMPV6_RTR_ADV;
    me_reg.target.pid = brr_pid;
    gnrc_netreg_register(GNRC_NETTYPE_ICMPV6, &me_reg);
    return 0;
}
