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

docs:
	cd doc/doxygen && make

-include makefiles/tests.inc.mk
