include ../Makefile.tests_common

test-boot:
	make -C $(RIOTBASE)/../ install-boot BOARD=$(BOARD) EXTERNAL_BOARD_DIRS=$(RIOTBASE)/../boards
	make -C $(RIOTBASE)/../examples/blinking update BOARD=$(BOARD)
