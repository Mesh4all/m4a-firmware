APPLICATION = mesh4all-rpl-dodag

# If We need custom actions, routines, etc **SHOULD BE** included in the "core" folder
# 	and "MUST BE" code hardware independent
# EXTERNAL_MODULE_DIRS += $(CURDIR)/../core
DEVHELP ?= 1

include ../Makefile.common

# Modules
USEMODULE += saul_default
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

USEMODULE += rpl_protocol
USEMODULE += radio
USEMODULE += gnrc_icmpv6_echo
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

# Set a custom channel if needed
include $(RIOTMAKE)/default-radio-settings.inc.mk
