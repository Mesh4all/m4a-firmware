APPLICATION = test_esp32
BOARD ?= esp32-wrover-kit

# Current path
APPBASE ?= $(CURDIR)

# RIOT-OS's base path
RIOTBASE ?= $(CURDIR)/../../RIOT

# Custom boards "MUST BE" into "boards" folder, it's code **strongly** hardware dependent
EXTERNAL_BOARD_DIRS ?= $(CURDIR)/../../boards

USEMODULE += saul_default
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += xtimer
USEMODULE += at
USEMODULE += at_urc_isr_medium

FEATURES_REQUIRED = periph_adc
USEMODULE += ztimer
USEMODULE += ztimer_msec
USEMODULE += printf_float

DEVHELP ?= 1
QUIET ?= 1

# Disable warnings when a parameter is not used
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-unused-variable

include $(RIOTBASE)/Makefile.include