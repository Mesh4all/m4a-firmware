ifndef APPBASE
APPBASE := $(RIOTBASE)/../firmware	#This refers to called m4a-firmware
endif

ifneq (,$(filter meshme vs203,$(BOARD)))
upload:
	- FEATURES_REQUIRED=riotboot $(MAKE_CMD) M4ABOOT=1 -C $(APPBASE) all riotboot/flash-slot0
else
upload:
	- FEATURES_REQUIRED=riotboot USEMODULE=usbus_dfu make M4ABOOT=1 -C $(APPBASE) all riotboot/flash-slot0
endif
