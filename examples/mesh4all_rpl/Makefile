APPLICATION = mesh4all-rpl
BOARD ?= m4a-24g

# Current path
APPBASE ?= $(CURDIR)

# RIOT-OS's base path
RIOTBASE ?= $(CURDIR)/../../RIOT

# Custom boards "MUST BE" into "boards" folder, it's code **strongly** hardware dependent
EXTERNAL_BOARD_DIRS ?= $(CURDIR)/../../boards

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

# Custom modules
# Note: this code is hardware independent but "COULD BE" configurable for a
#	target board

USEMODULE += rpl_protocol
USEMODULE += radio
USEMODULE += gnrc_icmpv6_echo
USEMODULE += at_client

EXTERNAL_MODULE_DIRS += $(CURDIR)/../../firmware/peripherals
EXTERNAL_MODULE_DIRS += $(CURDIR)/../../firmware/network
EXTERNAL_MODULE_DIRS += $(CURDIR)/../../firmware/sys

# Use UART_DEV(1) for slipdev as a default, baudrate by default is 115200


include $(RIOTBASE)/Makefile.include
