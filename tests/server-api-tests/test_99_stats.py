#!/usr/bin/env python3
##################################################################################
#                      Project
#    __ _
#   / _| |__   __ _       ___  ___ _ ____   _____ _ __
#  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
#  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
#  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
#                |_|
#
# MIT License
#
# Copyright (c) 2011-2026 FreeHackQuest <freehackquest@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Original repository: https://github.com/freehackquest/fhq-server
#
##################################################################################

"""
Show all statistics (must be run as latest)
"""

import math


def test_coverage(admin_session):
    """Coverage"""
    print(test_coverage.__doc__)
    print("Start calculating coverage")
    server_api = admin_session.server_api({})
    assert server_api is not None
    assert server_api["result"] == "DONE"

    server_info = admin_session.server_info({})
    assert server_info is not None
    assert server_info["result"] == "DONE"

    stats = {}
    all_cmd_count = 0
    called_cmd_count = 0
    for cmd_stat in server_api['data']:
        cmd = cmd_stat['cmd']
        all_cmd_count = all_cmd_count + 1
        stats[cmd] = 0
        if cmd in server_info['data']['request_statistics']:
            stats[cmd] = server_info['data']['request_statistics'][cmd]
            called_cmd_count = called_cmd_count + 1
        line_print = "Command " + cmd + " " + str(stats[cmd]) + " times"
        print(line_print)
    procents_api_coverage = str(math.floor(called_cmd_count*100/all_cmd_count))
    print("\nServer API Coverage: " + procents_api_coverage + "% \n", )
