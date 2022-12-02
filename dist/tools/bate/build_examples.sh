#!/bin/bash

: "${examples_dir:=$(cd $(dirname $0)/../../../examples; pwd)}"
cd $examples_dir || exit;

ERR_FILES=()
err=0
dirs=(${examples_dir}/*/)
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