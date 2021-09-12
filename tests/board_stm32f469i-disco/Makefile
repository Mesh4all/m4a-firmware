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
USEMODULE += xtimer

# Set log level:
# LOG_NONE:    Lowest log level, will output nothing
# LOG_ERROR;   Error log level, will print only critical, non-recoverable errors like hardware initialization failures
# LOG_WARNING; Warning log level, will print warning messages for temporary errors
# LOG_INFO;    Informational log level, will print purely informational messages like successful system bootup, network link state, etc
# LOG_DEBUG;   Debug log level, printing developer stuff considered too verbose for production use */
# LOG_ALL;     print everything
CFLAGS += -DLOG_LEVEL=LOG_DEBUG

include $(RIOTBASE)/Makefile.include