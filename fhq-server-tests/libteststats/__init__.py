#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from libfreehackquestclient import FreeHackQuestClient
import fhqtest
import sys, traceback
import base64
import os
import zipfile
import shutil
import json
from pprint import pprint

test_name = 'Statistics'

def run_tests():
    server_api = fhqtest.admin_session.server_api({})
    fhqtest.check_response(server_api, "problem with server_api")

    server_info = fhqtest.admin_session.server_info({})
    fhqtest.check_response(server_info, "problem with server_info")

    stats = {}
    all_cmd_count = 0
    called_cmd_count = 0
    for a in server_api['data']:
        cmd = a['cmd']
        all_cmd_count = all_cmd_count + 1
        stats[cmd] = 0
        if cmd in server_info['data']['request_statistics']:
            stats[cmd] = server_info['data']['request_statistics'][cmd]
            called_cmd_count = called_cmd_count + 1
        s = "Command " + cmd + " " + str(stats[cmd]) + " times"
        if stats[cmd] == 0:
            fhqtest.log_warn(s)
        else:
            fhqtest.print_success(s)

    fhqtest.print_bold("***********\n*API Coverage " + str(called_cmd_count*100/all_cmd_count) + "%\n***********")
