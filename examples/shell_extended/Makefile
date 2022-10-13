APPLICATION ?= shell_extended

include ../Makefile.common

# Include the shell extended module, shell and shell commands modules
# will be implicitly included.
USEMODULE += shell_commands
USEMODULE += shell_extended
# USEMODULE+= border_router

# Include desired modules where shell extended is enabled.
USEMODULE += radio
USEMODULE += uniqueid
USEMODULE += net_tools
USEMODULE += storage

include $(RIOTBASE)/Makefile.include
