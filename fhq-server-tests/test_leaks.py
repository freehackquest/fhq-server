#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Test leaks api
"""

import fhqtest
import libtestleaks
try:
    fhqtest.init_enviroment()
    libtestleaks.run_tests()
finally:
    fhqtest.deinit_enviroment()
