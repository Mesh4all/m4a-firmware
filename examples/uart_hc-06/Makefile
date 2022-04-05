APPLICATION = uart_HC-06

include ../Makefile.common

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
