#!/usr/bin/env bash
CHAMOC_DIR="$(dirname "$(readlink -f "$0")")"
: "${MESHBASE:="$(cd "$(dirname "$0")"/../../../ || exit; pwd)"}"
: "${RIOTTOOLS:="$(cd "${MESHBASE}"/RIOT/dist/tools || exit; pwd)"}"
: "${ETHOS_DIR:="$(cd "${RIOTTOOLS}"/ethos || exit; pwd)"}"

ETHOS_APP=${ETHOS_DIR}/ethos
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

tap_init_setup(){
    ${SUDO} ip tuntap add "${INTERFACE}" mode tap user "${USER}"
    ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".forwarding=1
    ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".accept_ra=0
    ${SUDO} ip link set "${INTERFACE}" up
    ${SUDO} ip a a "fe80::1"/"${PREFIX}" dev "${INTERFACE}"
    ${SUDO} ip a a "${HOST_IPV6}"/"${PREFIX}" dev "${INTERFACE}"
    gnome-terminal -- "${CHAMOC_DIR}"/chamoc_test_client nib add "${INTERFACE}" "${HOST_IPV6}" "${PREFIX}"
    ${ETHOS_APP} "${INTERFACE}" "${PORT}" "${ETHOS_BAUDRATE}"
}

remove_tap(){
    ${SUDO} ip tuntap del "${INTERFACE}" mode tap
    trap "" INT QUIT TERM EXIT
}

usb_cdc_init_setup(){
    ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".forwarding=1
    ${SUDO} sysctl -w net.ipv6.conf."${INTERFACE}".accept_ra=0
    ${SUDO} ip a a "${HOST_IPV6}"/"${PREFIX}" dev "${INTERFACE}"
    echo "Start sending a nib add request"
    ${SUDO} "${CHAMOC_DIR}"/chamoc_test_client nib add "${INTERFACE}" "${HOST_IPV6}" "${PREFIX}"
    ${SUDO} ip link set "${INTERFACE}" up
}

# these both functions provides
build_req_apps(){   #Works to build the chamoc app and the ethos app
    if [ ! -f "${CHAMOC_APP}" ]; then
        echo "Building chamos app"
        >&- make -C "${CHAMOC_DIR}"
    fi
    if [ ! -f "${ETHOS_APP}" ]; then
        echo "Building ethos app"
        >&- make -C "${ETHOS_DIR}"
    fi
}
remove_req_apps(){
    rm "${CHAMOC_DIR}"
    rm "${ETHOS_APP}"
}

CC=gcc  #Refer to gcc as C compiler to Linux OS applications
echo "${CC}" placed to build Linux applications.

build_req_apps
sleep 1
HOST_IPV6=$1
NETIFACE_TYPE=$2
if [ -z "${HOST_IPV6}" ]; then
    HOST_IPV6="2001:db8:1::1"
fi

if [ "${NETIFACE_TYPE}" == "USB" ]; then
find_interface
if [ -z "${INTERFACE}" ]; then
    echo "USB network interface not found"
    exit 0
else
usb_cdc_init_setup
fi
else
trap "remove_tap" INT QUIT TERM EXIT
INTERFACE=$3
PORT=$4
ETHOS_BAUDRATE=$5
tap_init_setup
fi

echo "Connection started"
