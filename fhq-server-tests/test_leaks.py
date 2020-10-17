#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
run leaks tests api
"""

import fhqtest
import tests_collection
try:
    fhqtest.init_enviroment()
    tests_collection.leaks.run_tests()
finally:
    fhqtest.deinit_enviroment()
