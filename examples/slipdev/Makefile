APPLICATION = slipdev_example

include ../Makefile.common

# Custom boards "MUST BE" into "boards" folder, it's code **strongly** hardware dependent

USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += auto_init_gnrc_netif
USEMODULE += slipdev
USEMODULE += slipdev_l2addr
USEMODULE += gnrc_ipv6
USEMODULE += gnrc_icmpv6_echo
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6

# Set GNRC_PKTBUF_SIZE via CFLAGS if not being set via Kconfig.
ifndef CONFIG_GNRC_PKTBUF_SIZE
  CFLAGS += -DCONFIG_GNRC_PKTBUF_SIZE=8192
endif

# Additional networking modules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_error

DEVHELP ?= 1
QUIET ?= 1

# Disable warnings when a parameter is not used
# CFLAGS += -Wno-unused-parameter
# CFLAGS += -Wno-unused-variable

# Set log level
SLIP_UART ?= 2
SLIP_BAUDRATE ?= 115200
CFLAGS += -DSLIP_UART="UART_DEV($(SLIP_UART))"
CFLAGS += -DSLIP_BAUDRATE=$(SLIP_BAUDRATE)
CFLAGS += -DLOG_LEVEL=LOG_DEBUG

CFLAGS += -I$(CURDIR)

include $(RIOTBASE)/Makefile.include
