APPLICATION = moisture_sensor_test
BOARD ?= m4a-wrover

# Current path
APPBASE ?= $(CURDIR)

# RIOT-OS's base path
RIOTBASE ?= $(CURDIR)/../../RIOT
EXTERNAL_BOARD_DIRS ?= $(CURDIR)/../../boards

USEMODULE += saul_default
USEMODULE += shell
USEMODULE += shell_commands
FEATURES_REQUIRED = periph_adc
USEMODULE += ztimer
USEMODULE += ztimer_msec
USEMODULE += printf_float

DEVHELP ?= 1
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
