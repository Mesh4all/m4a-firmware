
# With HomeBrew into MacOS systems make is installed as "gmake"

ifeq ($(shell uname -s), Darwin)
	MAKE_CMD := gmake
else
	MAKE_CMD := make
endif

#Bootloaders params

ifneq (,$(filter meshme vs203,$(BOARD)))	# Unsupported board without periph/usbdev (DFU not supported)
	BOOTLOADER_DIR := RIOT/bootloaders/riotboot
else
	BOOTLOADER_DIR := RIOT/bootloaders/riotboot_dfu
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
	- $(MAKE_CMD) EXTERNAL_BOARD_DIRS=$(CURDIR)/boards -C $(BOOTLOADER_DIR) flash
	@echo Please connect in the DFU mode. Change to the port Target and them run:
	@echo + make update

ifneq (,$(filter meshme vs203,$(BOARD)))
update:
	- FEATURES_REQUIRED=riotboot $(MAKE_CMD) M4ABOOT=1 -C firmware all riotboot/flash-slot0
else
update:
	- FEATURES_REQUIRED=riotboot USEMODULE=usbus_dfu $(MAKE_CMD) M4ABOOT=1 -C firmware all riotboot/flash-slot0
endif

docs:
	cd doc/doxygen && make

-include makefiles/tools/m4agen.inc.mk
-include makefiles/tests.inc.mk
