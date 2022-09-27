include ../Makefile.tests_common

USEMODULE += embunit
USEMODULE += storage
USEMODULE += radio
USEMODULE += rpl_protocol

ifeq (,$(filter native,$(BOARD)))
     USEMODULE += stdio_cdc_acm
endif

include $(RIOTBASE)/Makefile.include
