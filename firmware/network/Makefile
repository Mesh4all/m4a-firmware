ifneq (,$(filter udp_client,$(USEMODULE)))
  DIRS += udp_client
endif

ifneq (,$(filter udp_server,$(USEMODULE)))
  DIRS += udp_server
endif

ifneq (,$(filter radio,$(USEMODULE)))
  DIRS += radio
endif

ifneq (,$(filter rpl_protocol,$(USEMODULE)))
  DIRS += rpl_protocol
endif

include $(RIOTBASE)/Makefile.base
