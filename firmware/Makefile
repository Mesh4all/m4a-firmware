APPLICATION = m4a-mesh
BOARD ?= m4a-mb

# Current path
APPBASE ?= $(CURDIR)

# RIOT-OS's base path
RIOTBASE ?= $(CURDIR)/../RIOT

# Custom boards "MUST BE" into "boards" folder, it's code **strongly** hardware dependent
EXTERNAL_BOARD_DIRS ?= $(CURDIR)/../boards

# If We need custom actions, routines, etc **SHOULD BE** included in the "core" folder
# 	and "MUST BE" code hardware independent
# EXTERNAL_MODULE_DIRS += $(CURDIR)/../core

DEVHELP ?= 1
QUIET ?= 1

# Modules
USEMODULE += saul_default
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += netdev_default
USEMODULE += auto_init_gnrc_netif
# Activate ICMPv6 error messages
USEMODULE += gnrc_icmpv6_error
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_udp
# Add a routing protocol
USEMODULE += gnrc_rpl
USEMODULE += auto_init_gnrc_rpl
# Optional , p2p over rpl
USEMODULE += gnrc_rpl_p2p
# This application dumps received packets to STDIO using the pktdump module
USEMODULE += gnrc_pktdump
# Additional networking modules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo
USEMODULE += netutils
# Add some stats
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
USEMODULE += netstats_rpl

# Custom modules
# Note: this code is hardware independent but "COULD BE" configurable for a
#	target board
# EXTERNAL_MODULE_DIRS += external_drivers

CFLAGS += -DCONFIG_GNRC_RPL_DEFAULT_NETIF=6
#
CFLAGS += -DCONFIG_GNRC_IPV6_NIB_NUMOF=50
CFLAGS += -DCONFIG_GNRC_IPV6_NIB_OFFL_NUMOF=50
#
CFLAGS += -DCONFIG_GNRC_IPV6_NIB_DEFAULT_ROUTER_NUMOF=2

# Disable warnings when a parameter is not used
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-unused-variable

# Set log level:
# LOG_NONE:    Lowest log level, will output nothing
# LOG_ERROR;   Error log level, will print only critical, non-recoverable errors like hardware initialization failures
# LOG_WARNING; Warning log level, will print warning messages for temporary errors
# LOG_INFO;    Informational log level, will print purely informational messages like successful system bootup, network link state, etc
# LOG_DEBUG;   Debug log level, printing developer stuff considered too verbose for production use */
# LOG_ALL;     print everything
CFLAGS += -DLOG_LEVEL=LOG_ALL

include $(RIOTBASE)/Makefile.include