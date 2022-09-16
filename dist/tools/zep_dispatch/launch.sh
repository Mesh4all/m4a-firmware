#!/bin/bash

# Launcher to Foren6 nodes

ARRAY=(
    "C3:20:D8:BF:73:D0:F6:69"
    "A6:5A:49:45:82:62:C3:F2"
    "23:19:A4:BD:C2:1E:55:77"
    "2A:3A:AC:FA:9A:EF:48:5E"
    "10:A0:9E:04:71:15:6D:18"
)

ARR_LEN=${#ARRAY[@]}


: "${ZEP_DIR:=pwd}"
: "${DIR_APPS:=$(cd "$(dirname "$0")"/../../../examples || exit; pwd)}"
BOARD=native
gnome-terminal --window -- make run TOPOLOGY=file.topo

gnome-terminal --title=DODAG --tab -- make -C "${DIR_APPS}"/rpl_dodag BOARD="${BOARD}" ZEP_MAC="${ARRAY[0]}" term

for ((i=1; i<"${ARR_LEN}"; i++))
do
    gnome-terminal --title=DAG_"${i}" --tab -- make -C "${DIR_APPS}"/rpl_dag BOARD="${BOARD}" ZEP_MAC="${ARRAY[$i]}" term
    sleep 1;
done
