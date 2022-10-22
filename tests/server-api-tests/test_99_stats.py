#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2011-2022, Evgenii Sopov <mrseakg@gmail.com>

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
