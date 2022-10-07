MODULE = shell_extended

SRC += shell_extended.c

ifneq (,$(filter uniqueid,$(USEMODULE)))
  SRC += se_uniqueid.c
endif

ifneq (,$(filter storage,$(USEMODULE)))
  SRC += se_storage.c
endif

ifneq (,$(filter net_tools,$(USEMODULE)))
  SRC += se_net_tools.c
endif

include $(RIOTBASE)/Makefile.base
