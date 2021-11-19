# APPLICATION = test_driver_ds18
BOARD ?= m4a-mb

# Current path
APPBASE ?= $(CURDIR)

# RIOT-OS's base path
RIOTBASE ?= $(CURDIR)/../../RIOT

# Custom boards "MUST BE" into "boards" folder, it's code **strongly** hardware dependent
EXTERNAL_BOARD_DIRS ?= $(CURDIR)/../../boards

USEMODULE += ds18
USEMODULE += xtimer
USEMODULE += printf_float

DEVHELP ?= 1
QUIET ?= 1

# Disable warnings when a parameter is not used
# CFLAGS += -Wno-unused-parameter
# CFLAGS += -Wno-unused-variable

# Set log level
CFLAGS += -DLOG_LEVEL=LOG_DEBUG

include $(RIOTBASE)/Makefile.include