#!/bin/bash

: "${tests_dir:=$(cd $(dirname $0)/../../../tests; pwd)}"
cd $tests_dir || exit;

err=0
dirs=(${tests_dir}/*/)
ERR_FILES=()
status=0
for i in ${!dirs[@]}; do
    make -C ${dirs[i]}
    status=$?
    if ((${status}!=0)); then
        ERR_FILES+=(${dirs[i]})
        err=${status}
    fi
done

if ((${err}!=0)); then
    printf "\nError: Some files are presenting a built error:\n"
    for i in ${!ERR_FILES[@]}; do
        echo -e '\033[0;31m'${ERR_FILES[i]}
    done
    exit $err
else
    exit 0
fi