#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import fhqtest
import libtestquests
try:
    fhqtest.init_enviroment()
    libtestquests.run_tests()
finally:
    fhqtest.deinit_enviroment()