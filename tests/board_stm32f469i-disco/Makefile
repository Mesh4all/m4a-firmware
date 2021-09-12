APPLICATION = simple_test
BOARD ?= stm32f469i-disco

# Current path
APPBASE ?= $(CURDIR)

# RIOT-OS's base path
RIOTBASE ?= $(CURDIR)/../../RIOT

# Custom boards "MUST BE" into "boards" folder, it's code **strongly** hardware dependent
EXTERNAL_BOARD_DIRS ?= $(CURDIR)/../../boards

DEVHELP ?= 1
QUIET ?= 1

# Set log level
CFLAGS += -DLOG_LEVEL=LOG_DEBUG

include $(RIOTBASE)/Makefile.include