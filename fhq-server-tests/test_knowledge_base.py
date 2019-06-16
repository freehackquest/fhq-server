#!/usr/bin/env python
# -*- coding: utf-8 -*-


import fhqtest
import libtestknowledgebase

try:
    fhqtest.init_enviroment()
    libtestknowledgebase.run_tests()
finally:
    fhqtest.deinit_enviroment()