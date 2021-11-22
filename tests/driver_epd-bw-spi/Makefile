# APPLICATION = driver_epd_bw_spi
BOARD ?= m4a-mb

APPBASE ?= $(CURDIR)

RIOTBASE ?= $(CURDIR)/../../RIOT

EXTERNAL_BOARD_DIRS ?= $(CURDIR)/../../boards

DEVHELP ?= 1
QUIET ?= 1

USEMODULE += epd_bw_spi
USEMODULE += xtimer

EXTERNAL_MODULE_DIRS += external_drivers


test-clean:
	- cd bin && rm -rf *

include $(RIOTBASE)/Makefile.include