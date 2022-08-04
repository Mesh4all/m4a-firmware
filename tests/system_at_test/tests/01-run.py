#!/usr/bin/env python3
# Copyright (c) 2022 Mesh4all <mesh4all.org>
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# author       RocioRojas <rociorojas391@gmail.com>
import sys
from testrunner import run

PROMPT = '> '

CMDS = (
    ('at ath', 'ATH'),
    ('at at', 'AT'),
    ('at wsta_ssid TestTest', 'AT+WSTA_SSID=TestTest'),
    ('at ath0', 'ATH0'),
)

CMDS_REGEX = {'ps'}


def check_cmd(child, cmd, expected):
    regex = cmd in CMDS_REGEX
    child.expect(PROMPT)
    child.sendline(cmd)
    rxBuf = []
    for line in expected:
        if regex:
            child.expect(line)
        else:
            child.expect_exact(line)
            rxBuf.append(child.match)
            recv = ''.join(rxBuf)
    if(expected == recv):
        print("\nTest '" + cmd + "' OK\n")
    else:
        print("\nTest '" + cmd + "' FAILED\n")


def testfunc(child):
    for cmd, expected in CMDS:
        check_cmd(child, cmd, expected)


if __name__ == "__main__":
    sys.exit(run(testfunc, timeout=40))
