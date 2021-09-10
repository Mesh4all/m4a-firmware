APPLICATION = cdc_border_router

BOARD ?= nt-db

RIOTBASE ?= $(CURDIR)/../../RIOT

EXTERNAL_BOARD_DIRS ?= $(CURDIR)/../../boards


GNRC_NETIF_NUMOF := 2

USEMODULE += usbus_cdc_ecm
USEMODULE += auto_init_usbus

USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_icmpv6_error
USEMODULE += gnrc_sixlowpan_border_router_default
USEMODULE += gnrc_udp

USEMODULE += gnrc_rpl
USEMODULE += auto_init_gnrc_rpl

USEMODULE += gnrc_icmpv6_echo
USEMODULE += shell
USEMODULE += shell_commands
# Optional, but might be useful
USEMODULE += ps
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
USEMODULE += netstats_rpl

USB_VID ?= 03eb # Replace with your vendor ID
USB_PID ?= 2169 # Replace with your product ID

CFLAGS += -DUSB_CONFIG_VID=0x$(USB_VID) -DUSB_CONFIG_PID=0x$(USB_PID)

DEVELHELP ?= 1

include $(RIOTBASE)/Makefile.include