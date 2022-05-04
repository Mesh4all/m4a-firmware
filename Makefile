
# With HomeBrew into MacOS systems make is installed as "gmake"

ifeq ($(shell uname -s), Darwin)
	MAKE_CMD := gmake
else
	MAKE_CMD := make
endif

all:
	- $(MAKE_CMD) -C firmware

flash:
	- $(MAKE_CMD) -C firmware flash

term:
	- $(MAKE_CMD) -C firmware term

clean:
	- $(MAKE_CMD) -C firmware clean

menuconfig:
	- $(MAKE_CMD) -C firmware menuconfig

install-boot:
	- $(MAKE_CMD) EXTERNAL_BOARD_DIRS=$(CURDIR)/boards -C RIOT/bootloaders/riotboot_dfu flash
	@echo Please connect in the DFU mode. Change to the port Target and them run:
	@echo + make update

update:
	- FEATURES_REQUIRED=riotboot USEMODULE=usbus_dfu $(MAKE_CMD) M4ABOOT=1 -C firmware all riotboot/flash-slot0
docs:
	cd doc/doxygen && make

-include makefiles/tools/m4agen.inc.mk
-include makefiles/tests.inc.mk
