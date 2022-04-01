#!/bin/bash

: "${BASE:=$(cd $(dirname $0)/../../../; pwd)}"
cd $BASE || exit;

: "${TOOLS:=${BASE}/dist/tools}"

CODESPELL_CMD="codespell"
CODESPELL_OPT=" -c"
CODESPELL_OPT+=" -q 2"
CODESPELL_OPT+=" --check-hidden"
CODESPELL_OPT+=" --ignore-words ${TOOLS}/codespell/ignored_words.txt"
CODESPELL_OPT+=" --skip=RIOT,dist,build,bin,.git,css,html,js,m4a.doxyfile,makefiles"
CODESPELL_OPT+=" -D ${TOOLS}/codespell/dictionary_m4a.txt -D -"
ERRORS=$(${CODESPELL_CMD} ${CODESPELL_OPT})

if [ -n "${ERRORS}" ]
then
    echo "${ERRORS}"
    printf "Found some misspellings words, please fix them before continuing.\n"
    exit 1
else
    printf "Codespell return no errors.\n"
    exit 0
fi