# name of the application
APPLICATION = border_router
RIOTBASE = ../../RIOT
include $(CURDIR)/../Makefile.common


USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

USEMODULE += fmt
USEMODULE += gnrc
USEMODULE += border_router
USEMODULE += chamos
USEMODULE += rpl_protocol
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

HOST_IPV6 ?= 2001:db8::1
IFTYPE ?= ETHOS

CHAMOC_DIR ?= ../../dist/tools/chamoc
ETHOS_TOOL ?= $(RIOTBASE)/dist/tools/ethos

ifeq (${IFTYPE},ETHOS)
	TAP := tap0
	USEMODULE += stdio_ethos
	ETHOS_BAUDRATE ?= 115200
	FEATURES_REQUIRED += periph_uart
	CFLAGS += -DETHOS_BAUDRATE=$(ETHOS_BAUDRATE)
	TERMPROG ?= $(ETHOS_TOOL)/ethos
	TERMFLAGS ?= ${TAP} ${PORT} ${ETHOS_BAUDRATE}
else ifeq (USB,${IFTYPE})
	USEMODULE += usbus_cdc_ecm
	USEMODULE += auto_init_usbus
endif

install:
	@./${CHAMOC_DIR}/install.sh ${HOST_IPV6} \
								${IFTYPE}    \
								${TAP}		 \
								${PORT} 	 \
								${ETHOS_BAUDRATE}
uninstall:
	@./${CHAMOC_DIR}/uninstall.sh ${HOST_IPV6} \
								${IFTYPE}    \
								${TAP}		 \


include $(RIOTBASE)/Makefile.include
