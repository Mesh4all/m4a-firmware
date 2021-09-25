APPLICATION = test_mpu9150
BOARD ?= m4a-mb

# Current path
APPBASE ?= $(CURDIR)

# RIOT-OS's base path
RIOTBASE ?= $(CURDIR)/../../RIOT

# Custom boards "MUST BE" into "boards" folder, it's code **strongly** hardware dependent
EXTERNAL_BOARD_DIRS ?= $(CURDIR)/../../boards

DEVHELP ?= 1
QUIET ?= 1

USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_commands
#USEMODULE += mpu9150
USEMODULE += saul_default
FEATURES_REQUIRED += periph_gpio
#FEATURES_REQUIRED += periph_i2c


include $(RIOTBASE)/Makefile.include
