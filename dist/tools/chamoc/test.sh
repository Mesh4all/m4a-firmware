#!/usr/bin/env bash

TEST_APP="chamoc_test_client"

CC=gcc
CFLAGS=-o
INCLUDES_DIR=include
INCLUDE=-I${INCLUDES_DIR}
TEST_FILE=*.c

compile(){
    ${CC} ${INCLUDE} ${TEST_FILE} ${CFLAGS} ${TEST_APP}
}

run(){
    echo "running nib add request"
    ./${TEST_APP} nib add "${INTERFACE}" "${IPV6_ADD}" "${IPV6_PREFIX}"
}
closing(){
    echo "Sending nib del request"
    ./${TEST_APP} nib del "${INTERFACE}" "${IPV6_ADD}" "${IPV6_PREFIX}"
}

read -p "Interface: " -r INTERFACE
read -p "Ipv6 Address: " -r IPV6_ADD
read -p "Ipv6 Address prefix (64 - 128): " -r  IPV6_PREFIX
if [ -z "${INTERFACE}" ] || [ -z "${IPV6_ADD}" ] || [ -z "${IPV6_PREFIX}" ]; then
    echo "Any parameter cannot be empty"
else
compile
run
echo "Run again ./run.sh in server test, then Press any key to send a nib del request"
read -r -n 1
closing
fi
