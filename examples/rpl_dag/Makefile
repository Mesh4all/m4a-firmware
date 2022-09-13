APPLICATION = mesh4all-rpl

DEVHELP ?= 1

include ../Makefile.common

# Modules
USEMODULE += saul_default
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

# Custom modules
# Note: this code is hardware independent but "COULD BE" configurable for a
#	target board

USEMODULE += rpl_protocol
USEMODULE += gnrc_icmpv6_echo

# Use UART_DEV(1) for slipdev as a default, baudrate by default is 115200
# Instead of simulating an Ethernet connection, we can also simulate
# an IEEE 802.15.4 radio using ZEP
# set the ZEP port for native
ZEP_PORT_BASE ?= 17754

ifneq (,$(filter native,$(BOARD)))
	 USE_ZEP ?= 1
     ifeq (1,$(USE_ZEP))
		TERMFLAGS += -z [::1]:$(ZEP_PORT_BASE)
		USEMODULE += socket_zep

		ifneq (,$(ZEP_MAC))
		TERMFLAGS += --eui64=$(ZEP_MAC)
		endif
	endif
endif

include $(RIOTBASE)/Makefile.include
