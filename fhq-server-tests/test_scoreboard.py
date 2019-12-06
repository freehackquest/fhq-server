#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import fhqtest
import libtestscoreboard

try:
    fhqtest.init_enviroment()
    libtestscoreboard.run_tests()
finally:
    fhqtest.deinit_enviroment()