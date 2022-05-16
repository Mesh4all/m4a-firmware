#!/usr/bin/env bash

TEST_APP="chamoc_server"

CC=gcc
CFLAGS=-o
INCLUDES_DIR=../../dist/tools/chamoc/include
INCLUDE=-I${INCLUDES_DIR}
TEST_FILE=chamoc_server.c
SERVER_PORT=6977

compile(){
    ${CC} ${INCLUDE} ${TEST_FILE} ${CFLAGS} ${TEST_APP}
}

run(){
    ./${TEST_APP} ${INTERFACE} ${SERVER_PORT}
}

read -p "Interface: " -r INTERFACE
if [ -z "${INTERFACE}" ]; then
    echo "Any parameter cannot be empty"
else
compile
run
fi
