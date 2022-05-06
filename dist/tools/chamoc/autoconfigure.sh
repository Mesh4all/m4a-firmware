#!/usr/bin/env bash

SUDO=${SUDO:-sudo}
PREFIX=64
NPREFIX=::

unsupported_platform() {
    echo "unsupported platform" >&2
    echo "(currently supported \`uname -s\` 'Darwin' and 'Linux')" >&2
}

case "$(uname -s)" in
    Darwin)
        PLATFORM="OSX";;
    Linux)
        PLATFORM="Linux";;
    *)
        unsupported_platform
        exit 1
        ;;
esac

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
    INTERFACE=${INTERFACE%/}
    echo ${INTERFACE}
}

echo "Waiting for network interface."
find_interface

add_addresses(){
    case "${PLATFORM}" in
        Linux)
            ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".forwarding=1
            ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".accept_ra=0
            ${SUDO} ip link set "${INTERFACE}" up
            ${SUDO} ip a a "${IPV6_GLOBAL}"/64 dev "${INTERFACE}"
            ${SUDO} ip route add "${NPREFIX}" via "${IPV6_GLOBAL}" dev "${INTERFACE}"
            gcc -Iinclude *.c -o chamoc_test_client
            echo "Start sending a nib add request"
            ${SUDO} ./chamoc_test_client nib add "${INTERFACE}" "${IPV6_GLOBAL}" "${PREFIX}"
            ;;
        OSX)
            ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".forwarding=1
            ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".accept_ra=0
            ${SUDO} ip link set "${INTERFACE}" up
            ${SUDO} ip a a "${IPV6_GLOBAL}"/64 dev "${INTERFACE}"
            ${SUDO} ip route add "${NPREFIX}" via "${IPV6_GLOBAL}" dev "${INTERFACE}"
            gcc -Iinclude *.c -o chamoc_test_client
            echo "Start sending a nib add request"
            ${SUDO} ./chamoc_test_client nib add "${INTERFACE}" "${IPV6_GLOBAL}" "${PREFIX}"
            ;;
    esac
}

close_connection(){
            echo "Start sending a nib delete request"
            ${SUDO} ./chamoc_test_client nib del "${INTERFACE}" "${IPV6_GLOBAL}" "${PREFIX}"
            ${SUDO} ip link set "${INTERFACE}" up
            ${SUDO} ip a d "${IPV6_GLOBAL}"/64 dev "${INTERFACE}"
            ${SUDO} ip route del "${NPREFIX}" via "${IPV6_GLOBAL}" dev "${INTERFACE}"
            ${SUDO} ip neigh flush dev "${INTERFACE}"
}

IPV6_GLOBAL=$1

if [ -z "${IPV6_GLOBAL}" ]; then
    IPV6_GLOBAL="2001:db8:1::1"
fi

find_interface

if [ -z "${INTERFACE}" ]; then
   echo "USB network interface not found"
else
    add_addresses
    echo "Connection started"
    echo "Press any key to close connection"
    read -r -n 1
    close_connection
fi
