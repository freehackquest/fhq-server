#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import subprocess
import fhqtest
import signal

fhqtest.print_header(" > > > TESTS: begin ")

fhqtest.print_bold("Start fhq-server... ")
wd = os.getcwd()
print(wd)
os.chdir(wd + "/../fhq-server")
p_fhq_server = subprocess.Popen([wd + '/../fhq-server/fhq-server', 'start'])
os.chdir(wd)

try:
    def run_test(file_name):
        p_test = subprocess.Popen(['python', file_name])
        p_test.wait()
        if p_test.returncode != 0:
            sys.exit(-1)

    # run_test('test_users.py')
    run_test('test_games.py')
    run_test('test_leaks.py')
    
except Exception as e:
    fhqtest.log_err(str(e))
    traceback.print_exc(file=sys.stdout)
    exit(-1)
finally:
    fhqtest.print_header(" > > > TESTS: end ")
    print("Kill process " + str(p_fhq_server.pid))
    os.kill(p_fhq_server.pid, signal.SIGKILL)
