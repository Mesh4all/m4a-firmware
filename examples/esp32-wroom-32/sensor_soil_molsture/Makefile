# name of your application
APPLICATION = soil-molsture-test
# If no BOARD is found in the environment, use this default:
BOARD ?= esp32-wroom-32

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT
USEMODULE += periph_adc
# we want to use SAUL:
USEMODULE += saul_default
# include the shell:
USEMODULE += shell
USEMODULE += shell_commands
# additional modules for debugging:
USEMODULE += ps
USEMODULE += ztimer
USEMODULE += xtimer


EXTERNAL_BOARD_DIRS = $(CURDIR)/../../../boards

include $(RIOTBASE)/Makefile.include
