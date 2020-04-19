#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import fhqtest
import sys, traceback
import base64
import json
import libtestcommunication

try:
    fhqtest.init_enviroment()
    libtestcommunication.run_tests()
finally:
    fhqtest.deinit_enviroment()