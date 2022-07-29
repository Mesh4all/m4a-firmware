# name of the application
APPLICATION = border_router

include $(CURDIR)/../Makefile.common

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

USEMODULE += border_router
USEMODULE += rpl_protocol
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

HOST_IPV6 = 2001:db8::1
TERMPROG ?= sudo sh ${BORDER_ROUTER_USBTOOL}/autoconfigure.sh
TERMFLAGS ?= $(HOST_IPV6) $(PORT) ${RIOTTOOLS} ${CONFIG_ADDRESS_IPV6_WIRED}
BORDER_ROUTER_USBTOOL ?= $(RIOTBASE)/../dist/tools/usb-cdc-ecm

include $(RIOTBASE)/Makefile.include
