APPLICATION = m4a-mesh
BOARD ?= m4a-24g

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

# Custom modules
# Note: this code is hardware independent but "COULD BE" configurable for a
#	target board
USEMODULE += storage
USEMODULE += rpl_protocol
PSEUDOMODULES += 6lpan_node

EXTERNAL_MODULE_DIRS += network
EXTERNAL_MODULE_DIRS += sys

# Use UART_DEV(1) for slipdev as a default, baudrate by default is 115200
SLIP_UART ?= 1
CFLAGS += "-DSLIPDEV_PARAM_UART=UART_DEV($(SLIP_UART))"

# Set log level:
# LOG_NONE:    Lowest log level, will output nothing
# LOG_ERROR;   Error log level, will print only critical, non-recoverable errors like hardware initialization failures
# LOG_WARNING; Warning log level, will print warning messages for temporary errors
# LOG_INFO;    Informational log level, will print purely informational messages like successful system bootup, network link state, etc
# LOG_DEBUG;   Debug log level, printing developer stuff considered too verbose for production use */
# LOG_ALL;     print everything
CFLAGS += -DLOG_LEVEL=LOG_ALL

-include ../makefiles/tools/firmupdater.mk
-include $(RIOTBASE)/Makefile.include
