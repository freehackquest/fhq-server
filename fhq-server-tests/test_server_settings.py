#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import fhqtest
import libtestserversettings
try:
    fhqtest.init_enviroment()
    libtestserversettings.run_tests()
finally:
    fhqtest.deinit_enviroment()