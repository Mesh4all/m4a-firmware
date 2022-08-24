ifndef RIOTBASE
RIOTBASE ?= RIOT
endif

ifneq (,$(filter meshme vs203,$(BOARD)))	# Unsupported board without periph/usbdev (DFU not supported)
	BOOTLOADER_DIR ?= $(RIOTBASE)/bootloaders/riotboot
else
	BOOTLOADER_DIR ?= $(RIOTBASE)/bootloaders/riotboot_dfu
endif

install-bootloader:
	make -C ${BOOTLOADER_DIR} flash
	@echo Please connect in the DFU mode. Change to the port Target and them run:
	@echo + make upload
