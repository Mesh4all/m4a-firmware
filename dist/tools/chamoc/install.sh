#!/usr/bin/env bash
CHAMOC_DIR="$(dirname "$(readlink -f "$0")")"

SUDO=${SUDO:-sudo}
PREFIX=128
CHAMOC_APP=${CHAMOC_DIR}/chamoc_test_client

unsupported_platform() {
    echo "unsupported platform" >&2
    echo "(currently supported \`uname -s\` 'Darwin' and 'Linux')" >&2
}

INTERFACE_CHECK_COUNTER=5  # 5 attempts to find usb interface

find_interface() {
    INTERFACE=$(ls -A /sys/bus/usb/drivers/cdc_ether/*/net/ 2>/dev/null)
    INTERFACE_CHECK=$(echo -n "${INTERFACE}" | head -c1 | wc -c)
    if [ "${INTERFACE_CHECK}" -eq 0 ] && [ ${INTERFACE_CHECK_COUNTER} != 0 ]; then
        # We want to have multiple opportunities to find the USB interface
        # as sometimes it can take a few seconds for it to enumerate after
        # the device has been flashed.
        sleep 1
        ((INTERFACE_CHECK_COUNTER=INTERFACE_CHECK_COUNTER-1))
        find_interface
    fi
    INTERFACE="${INTERFACE%/}"
    echo "${INTERFACE}"
}

echo "Waiting for network interface."

add_addresses(){
            ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".forwarding=1
            ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".accept_ra=0
            ${SUDO} ip a a "${HOST_IPV6}"/"${PREFIX}" dev "${INTERFACE}"
            echo "Start sending a nib add request"
            ${SUDO} "${CHAMOC_DIR}"/chamoc_test_client nib add "${INTERFACE}" "${HOST_IPV6}" "${PREFIX}"
            ${SUDO} ip link set "${INTERFACE}" up
}

HOST_IPV6=$1
if [ -z "${HOST_IPV6}" ]; then
    HOST_IPV6="2001:db8:1::1"
fi

find_interface
# trap "close_connection" INT QUIT TERM EXIT

if [ -z "${INTERFACE}" ]; then
   echo "USB network interface not found"
   exit 1
fi

if [ ! -f "${CHAMOC_APP}" ]; then
    make -C "${CHAMOC_DIR}"
fi

add_addresses
rm "${CHAMOC_APP}"
echo "Connection started"
