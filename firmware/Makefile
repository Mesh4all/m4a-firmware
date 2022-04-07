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
EXTERNAL_MODULE_DIRS += sys

DEVHELP ?= 1
QUIET ?= 1

# Modules Wifi-subsystem
ifdef CONFIG_WIFI_SUBSYSTEM
USEMODULE+=at_client
endif

# Modules Radio-IEEE802.15.4
ifdef CONFIG_IEEE802154
USEMODULE += netdev_ieee802154
USEMODULE += netdev_eth
USEMODULE += auto_init_gnrc_netif
USEMODULE += netdev_default
USEMODULE += auto_init_gnrc_netif_ieee802154
endif

# Modules
USEMODULE += saul_default
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

# Custom modules
# Note: this code is hardware independent but "COULD BE" configurable for a
#	target board

USEMODULE += storage
USEMODULE += uniqueid


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

include $(RIOTBASE)/Makefile.include
