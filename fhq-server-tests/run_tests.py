#!/usr/bin/env python3
# -*- coding: utf-8 -*-

""" Run tests """

import sys
import fhqtest
import libtestfhq
import libtestwebserver
import libtestusers
import libtestpublicevents
import libteststats
import libtestquests
import libtestknowledgebase

TESTS_PASSED = False
TESTS = [
    libtestwebserver,
    libtestusers,
    libtestquests,
    libtestpublicevents,
    libtestknowledgebase,
    libteststats
]

try:
    fhqtest.print_header(" > > > TESTS: begin ")
    libtestfhq.start_server()
    fhqtest.init_enviroment()
    for t in TESTS:
        fhqtest.print_header(" > > > " + t.TEST_NAME + ": begin ")
        try:
            t.run_tests()
        except Exception as e:
            fhqtest.deinit_enviroment()
            fhqtest.throw_err("Some tests wrong")
            sys.exit(-1)
        fhqtest.print_header(" > > > " + t.TEST_NAME + ": end ")
    TESTS_PASSED = True
finally:
    fhqtest.print_header(" > > > TESTS: end ")
    if TESTS_PASSED:
        fhqtest.print_success("All tests passed")
    else:
        fhqtest.log_err("Some tests failed")
    libtestfhq.stop_server()