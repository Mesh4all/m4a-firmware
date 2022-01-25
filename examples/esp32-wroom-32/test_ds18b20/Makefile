# name of your application
APPLICATION = ds18b20_example

# If no BOARD is found in the environment, use this default:
BOARD ?= m4a-wroom

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT
EXTERNAL_BOARD_DIRS ?= $(CURDIR)/../../../boards
# we want to use SAUL:
USEMODULE += saul_default
# include the shell:
USEMODULE += ds18
USEMODULE += shell
USEMODULE += shell_commands
# additional modules for debugging:
USEMODULE += ps
USEMODULE += ztimer
USEMODULE += xtimer

include $(RIOTBASE)/Makefile.include
