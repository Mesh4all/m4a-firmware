#!/usr/bin/env bash
CHAMOC_DIR="$(dirname "$(readlink -f "$0")")"

SUDO=${SUDO:-sudo}
PREFIX=128
NPREFIX=::
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

close_connection(){
    echo "Start sending a nib delete request"
    ${SUDO} "${CHAMOC_DIR}"/chamoc_test_client nib del "${INTERFACE}" "${HOST_IPV6}" "${PREFIX}"
    ${SUDO} ip link set "${INTERFACE}" up
    ${SUDO} ip a d "${HOST_IPV6}"/"${PREFIX}" dev "${INTERFACE}"
    ${SUDO} ip route del "${NPREFIX}"/0 dev "${INTERFACE}"
    ${SUDO} ip neigh flush dev "${INTERFACE}"
    trap "" INT QUIT TERM EXIT
}

HOST_IPV6=$1
USB_IPV6=$2
if [ -z "${HOST_IPV6}" ]; then
    HOST_IPV6="2001:db8::1"
fi
if [ -z "${USB_IPV6}" ]; then
    USB_IPV6="2001:db8:1::2"
fi
find_interface

if [ -z "${INTERFACE}" ]; then
   echo "USB network interface not found"
   exit 1
fi

if [ ! -f "${CHAMOC_APP}" ]; then
    make -C "${CHAMOC_DIR}"
fi
close_connection
rm "${CHAMOC_APP}"
echo "Removed Connection"
