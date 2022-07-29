#!/usr/bin/env bash
USB_CDC_ECM_DIR="$(dirname "$(readlink -f "$0")")"

SUDO=${SUDO:-sudo}
PREFIX=64
NPREFIX=::
CHAMOC_APP=${USB_CDC_ECM_DIR}/chamoc_test_client

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
    INTERFACE="${INTERFACE%/}"
    echo "${INTERFACE}"
}

echo "Waiting for network interface."

add_addresses(){
    case "${PLATFORM}" in
        Linux)
            ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".forwarding=1
            ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".accept_ra=0
            ${SUDO} ip link set "${INTERFACE}" up
            ${SUDO} ip a a "${IPV6_GLOBAL}"/64 dev "${INTERFACE}"
            ${SUDO} ip route add "${NPREFIX}" via "${IPV6_GLOBAL}"/64 dev "${INTERFACE}"
            echo "Start sending a nib add request"
            ${SUDO} "${USB_CDC_ECM_DIR}"/chamoc_test_client nib add "${INTERFACE}" "${IPV6_GLOBAL}" "${PREFIX}"
            ;;
        OSX)
            ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".forwarding=1
            ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".accept_ra=0
            ${SUDO} ip link set "${INTERFACE}" up
            ${SUDO} ip a a "${IPV6_GLOBAL}"/64 dev "${INTERFACE}"
            ${SUDO} ip route add "${NPREFIX}" via "${IPV6_GLOBAL}"/64 dev "${INTERFACE}"
            echo "Start sending a nib add request"
            ${SUDO} "${USB_CDC_ECM_DIR}"/chamoc_test_client nib add "${INTERFACE}" "${IPV6_GLOBAL}" "${PREFIX}"
            ;;
    esac
}

close_connection(){
            echo "Start sending a nib delete request"
            ${SUDO} "${USB_CDC_ECM_DIR}"/chamoc_test_client nib del "${INTERFACE}" "${IPV6_GLOBAL}" "${PREFIX}"
            ${SUDO} ip link set "${INTERFACE}" up
            ${SUDO} ip a d "${IPV6_GLOBAL}"/64 dev "${INTERFACE}"
            ${SUDO} ip route del "${NPREFIX}" via "${IPV6_GLOBAL}" dev "${INTERFACE}"
            ${SUDO} ip neigh flush dev "${INTERFACE}"
            trap "" INT QUIT TERM EXIT
}

IPV6_GLOBAL=$1

if [ -z "${IPV6_GLOBAL}" ]; then
    IPV6_GLOBAL="2001:db8:1::1"
fi

find_interface
trap "close_connection" INT QUIT TERM EXIT

if [ -z "${INTERFACE}" ]; then
   echo "USB network interface not found"
   exit 1
fi

if [ -f "${CHAMOC_APP}" ]; then
    echo "Chamoc App found"
else
    echo "Chamoc App doesn´t exist, start building"
    ${SUDO} make -C "${USB_CDC_ECM_DIR}"
fi

add_addresses
echo "Connection started"
echo "Press any key to close connection"

PORT=$2
RIOTTOOLS_DIR=$3
if [ -z "${PORT}" ]; then
    echo "Network enabled over CDC-ECM"
    echo "Press Return to stop"
else
    "${RIOTTOOLS_DIR}/pyterm/pyterm" -p "${PORT}"
fi
