# APPLICATION = test_periph_uart
BOARD ?= stm32f469i-disco

# Current path
APPBASE ?= $(CURDIR)

# RIOT-OS's base path
RIOTBASE ?= $(CURDIR)/../../RIOT

# Custom boards "MUST BE" into "boards" folder, it's code **strongly** hardware dependent
EXTERNAL_BOARD_DIRS ?= $(CURDIR)/../../boards

USEMODULE += shell
USEMODULE += xtimer
USEMODULE += saul_default

FEATURES_REQUIRED += periph_uart

DEVHELP ?= 1
QUIET ?= 1

# Disable warnings when a parameter is not used
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-unused-variable

# Set log level
CFLAGS += -DLOG_LEVEL=LOG_DEBUG

include $(RIOTBASE)/Makefile.include