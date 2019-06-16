#!/usr/bin/env python
# -*- coding: utf-8 -*-

import fhqtest
import libtestpublicevents
try:
    fhqtest.init_enviroment()
    libtestpublicevents.run_tests()
finally:
    fhqtest.deinit_enviroment()