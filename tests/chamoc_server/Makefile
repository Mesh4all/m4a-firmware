TEST_APP="chamoc_server"

CC := gcc
CFLAGS := -o
CHAMOC_DIR := ../../dist/tools/chamoc
INCLUDES_DIR := ${CHAMOC_DIR}/include
INCLUDE := -I${INCLUDES_DIR}
TEST_FILE := chamoc_server.c
SERVER_PORT := 6977

INTERFACE := enp2s0
IPV6_ADD  := 2001:db8::1
IPV6_PREFIX := 64

all:
	${CC} ${INCLUDE} ${TEST_FILE} ${CFLAGS} ${TEST_APP}
	make -C ${CHAMOC_DIR}
test: all
	gnome-terminal --tab -t nib_add_received -- ./chamoc_server ${INTERFACE} ${SERVER_PORT};
	./${CHAMOC_DIR}/chamoc_test_client nib add "${INTERFACE}" "${IPV6_ADD}" "${IPV6_PREFIX}"
	gnome-terminal --tab -t nib_del_received -- ./chamoc_server ${INTERFACE} ${SERVER_PORT};
	./${CHAMOC_DIR}/chamoc_test_client nib del "${INTERFACE}" "${IPV6_ADD}" "${IPV6_PREFIX}"
