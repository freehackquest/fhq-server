#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import os
import subprocess
import fhqtest
import time
import signal

test_name = 'Start fhq-server'
p_fhq_server = None

def start_server():
    fhqtest.print_header("Start fhq-server")
    global p_fhq_server
    wd = os.getcwd()
    print(wd)
    os.chdir(wd + "/../fhq-server")
    p_fhq_server = subprocess.Popen([wd + '/../fhq-server/fhq-server', '-wd', '../data_travis', 'start'])
    os.chdir(wd)

    wait_max = 20
    wait_i = 0
    result_check_port = False
    while wait_i < wait_max:
        wait_i = wait_i + 1
        time.sleep(1)
        result_check_port = fhqtest.check_port('127.0.0.1', 1234)
        if not result_check_port:
            print(" =====> " + str(wait_i) + ": port not available... ")
        else:
            break

    if not result_check_port:
        print("Port not available... failed")
        exit(-1)
    else:
        print("Port available... OK!")

def stop_server():
    fhqtest.print_header("Stop fhq-server")
    global p_fhq_server
    if p_fhq_server != None:
        print("Kill process " + str(p_fhq_server.pid))
        os.kill(p_fhq_server.pid, signal.SIGKILL)