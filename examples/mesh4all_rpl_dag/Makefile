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

include $(RIOTBASE)/Makefile.include
