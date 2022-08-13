APPLICATION = mesh4all-rpl-dodag

# If We need custom actions, routines, etc **SHOULD BE** included in the "core" folder
# 	and "MUST BE" code hardware independent
# EXTERNAL_MODULE_DIRS += $(CURDIR)/../core

DEVHELP ?= 1

include ../Makefile.common
# Modules
USEMODULE += saul_default
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

USEMODULE += rpl_protocol
USEMODULE += radio
USEMODULE += gnrc_icmpv6_echo
USEMODULE += at_client


include $(RIOTBASE)/Makefile.include
