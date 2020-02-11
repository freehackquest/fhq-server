#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import os
import subprocess
import fhqtest
import signal
import time
import traceback
import libtestfhq
import libtestwebserver
import libtestusers
import libtestscoreboard
import libtestpublicevents
import libteststats
import libtestquests
import libtestleaks
import libtestgames
import libtestknowledgebase
import libtestserversettings

tests_passed = False
tests = [
    libtestwebserver,
    libtestusers,
    libtestgames,
    libtestquests,
    libtestscoreboard,
    libtestpublicevents,
    libtestleaks,
    libtestknowledgebase,
    libtestserversettings,
    libteststats
]

try:
    fhqtest.print_header(" > > > TESTS: begin ")
    libtestfhq.start_server()
    fhqtest.init_enviroment()
    for t in tests:
        fhqtest.print_header(" > > > " + t.test_name + ": begin ")
        try:
            t.run_tests()
        except Exception as e:
            fhqtest.deinit_enviroment()
            fhqtest.throw_err("Some tests wrong")
            exit(-1)
        fhqtest.print_header(" > > > " + t.test_name + ": end ")
    tests_passed = True
finally:
    fhqtest.print_header(" > > > TESTS: end ")
    if tests_passed:
        fhqtest.print_success("All tests passed")
    else:
        fhqtest.log_err("Some tests failed")
    libtestfhq.stop_server()