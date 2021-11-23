# APPLICATION = u8g2_test
BOARD ?= m4a-mb

# Current path
APPBASE ?= $(CURDIR)

# RIOT-OS's base path
RIOTBASE ?= $(CURDIR)/../../RIOT

# Custom boards "MUST BE" into "boards" folder, it's code **strongly** hardware dependent
EXTERNAL_BOARD_DIRS ?= $(CURDIR)/../../boards

DEVHELP ?= 1
QUIET ?= 1

USEMODULE += xtimer
USEPKG += u8g2
FEATURES_REQUIRED += periph_gpio
FEATURES_REQUIRED += periph_i2c

# set default device parameters in case they are undefined
TEST_OUTPUT ?= 4

TEST_I2C ?= 0

# for 128x32
TEST_ADDR ?= 0x3c

TEST_PIN_CS ?= GPIO_UNDEF
TEST_PIN_DC ?= GPIO_UNDEF
TEST_PIN_RESET ?= GPIO_UNDEF

# SSD1306 128x32
TEST_DISPLAY ?= u8g2_Setup_ssd1306_i2c_128x32_univision_1


CFLAGS += -DTEST_I2C=$(TEST_I2C)

CFLAGS += -DTEST_ADDR=$(TEST_ADDR)

CFLAGS += -DTEST_DISPLAY=$(TEST_DISPLAY)

# Disable warnings when a parameter is not used
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-unused-variable

# Set log level
CFLAGS += -DLOG_LEVEL=LOG_DEBUG

include $(RIOTBASE)/Makefile.include