#!/usr/bin/env python
# -*- coding: utf-8 -*-

import fhqtest
import libteststats

try:
    fhqtest.init_enviroment()
    libteststats.run_tests()
finally:
    fhqtest.deinit_enviroment()