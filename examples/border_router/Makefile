# name of the application
APPLICATION = border_router_example

include $(CURDIR)/../Makefile.common

USEMODULE += border_router
USEMODULE += radio
USEMODULE += shell
USEMODULE += ps
USEMODULE += shell_commands

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
