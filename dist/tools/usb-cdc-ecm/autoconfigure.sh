#!/usr/bin/env bash

SUDO=${SUDO:-sudo}
NPREFIX=::

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
        ${SUDO} ip link set "${INTERFACE}" up
        ${SUDO} ip a a "${IPV6_GLOBAL}"/64 dev "${INTERFACE}"
        ${SUDO} ip route add "${NPREFIX}"/0 via "${IPV6_USB_DEV}"  dev "${INTERFACE}"
        echo "It was assigned all network parameters"
}

close_connection(){
        ${SUDO} ip link set "${INTERFACE}" up
        ${SUDO} ip a d "${IPV6_GLOBAL}"/64 dev "${INTERFACE}"
        ${SUDO} ip route del "${NPREFIX}"/0 via "${IPV6_USB_DEV}" dev "${INTERFACE}"
        ${SUDO} ip neigh flush dev "${INTERFACE}"
        echo "It was removed all network parameters"
        trap "" INT QUIT TERM EXIT
}

find_interface

IPV6_GLOBAL=$1

if [ -z "${IPV6_GLOBAL}" ]; then
    IPV6_GLOBAL="2001:db8:1::1"
fi

trap "close_connection" INT QUIT TERM EXIT

if [ -z "${INTERFACE}" ]; then
   echo "USB network interface not found"
   exit 1
fi

IPV6_USB_DEV=$4
if [ -z "${IPV6_USB_DEV}" ]; then
    IPV6_USB_DEV="2001:db8::2"
fi

add_addresses
echo "Connection started"

PORT=$2
RIOTTOOLS_DIR=$3

if [ -z "${PORT}" ]; then
    echo "Network enabled over CDC-ECM"
    echo "Press Return to stop"
else
    "${RIOTTOOLS_DIR}/pyterm/pyterm" -p "${PORT}"
fi
