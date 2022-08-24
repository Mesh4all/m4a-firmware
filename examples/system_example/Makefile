APPLICATION = system-mesh
BOARD ?= m4a-24g

include ../Makefile.common

DEVHELP ?= 1

# Modules
USEMODULE += saul_default
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

# Custom modules
# Note: this code is hardware independent but "COULD BE" configurable for a
#	target board
USEMODULE += embunit
USEMODULE += uniqueid
USEMODULE += rpl_protocol
USEMODULE += radio
USEMODULE += serialization
USEMODULE += ds18_sensor
USEMODULE += gnrc_icmpv6_echo
USEMODULE += udp_client
USEMODULE += udp_server
USEMODULE += at_client

# Use UART_DEV(1) for slipdev as a default, baudrate by default is 115200

include $(RIOTBASE)/Makefile.include
