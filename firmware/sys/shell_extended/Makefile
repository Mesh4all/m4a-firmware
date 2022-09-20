MODULE = shell_extended

SRC += shell_extended.c

ifneq (,$(filter uniqueid,$(USEMODULE)))
  SRC += se_uniqueid.c
endif

include $(RIOTBASE)/Makefile.base
