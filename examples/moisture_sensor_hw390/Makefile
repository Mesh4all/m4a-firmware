APPLICATION = test_esp32
BOARD ?= esp32-wrover-kit

# Current path
APPBASE ?= $(CURDIR)

# RIOT-OS's base path
RIOTBASE ?= $(CURDIR)/../../RIOT

USEMODULE += saul_default
USEMODULE += shell
USEMODULE += shell_commands
FEATURES_REQUIRED = periph_adc
USEMODULE += ztimer
USEMODULE += ztimer_msec
USEMODULE += printf_float

# If We need custom actions, routines, etc **SHOULD BE** included in the "core" folder
# and "MUST BE" code hardware independent
# EXTERNAL_MODULE_DIRS += $(CURDIR)/../core

DEVHELP ?= 1
QUIET ?= 1

# Custom modules
# Note: this code is hardware independent but "COULD BE" configurable for a
# target board
# EXTERNAL_MODULE_DIRS += external_drivers

# Disable warnings when a parameter is not used


# Set log level:
# LOG_NONE:    Lowest log level, will output nothing
# LOG_ERROR;   Error log level, will print only critical, non-recoverable errors like hardware initialization failures
# LOG_WARNING; Warning log level, will print warning messages for temporary errors
# LOG_INFO;    Informational log level, will print purely informational messages like successful system bootup, network link state, etc
# LOG_DEBUG;   Debug log level, printing developer stuff considered too verbose for production use */
# LOG_ALL;     print everything
CFLAGS += -DLOG_LEVEL=LOG_ALL

include $(RIOTBASE)/Makefile.include
