CFLAGS += -DGNRC_RPL_LIFETIME_UNIT=1
CFLAGS += -DGNRC_RPL_DEFAULT_LIFETIME=32
CFLAGS += -DGNRC_RPL_REGULAR_DAO_INTERVAL=13
CFLAGS += -DGNRC_RPL_DEFAULT_DIO_INTERVAL_DOUBLINGS=13

# Currently only supports storing mode. That means, in order to have downwards routes
# to all nodes the storage space within gnrc_ipv6's Neighbor Information Base must be
# big enough to store information for each node.
#
# For a random topology of n nodes, to ensure you can reach every node from the root,
# set CONFIG_GNRC_IPV6_NIB_NUMOF == CONFIG_GNRC_IPV6_NIB_OFFL_NUMOF == n.
# Ref: https://doc.riot-os.org/group__net__gnrc__rpl.html
CFLAGS += -DCONFIG_GNRC_IPV6_NIB_NUMOF=50
CFLAGS += -DCONFIG_GNRC_IPV6_NIB_OFFL_NUMOF=50

# Allow alternative parents, increase num of default routers in the NIB.
# Ref: https://doc.riot-os.org/group__net__gnrc__rpl.html
CFLAGS += -DCONFIG_GNRC_IPV6_NIB_DEFAULT_ROUTER_NUMOF=2

include $(RIOTBASE)/Makefile.base
