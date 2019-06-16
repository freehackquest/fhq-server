#!/usr/bin/env python
# -*- coding: utf-8 -*-

import libfhqcli
import fhqtest
import sys, traceback
import base64
import os
import zipfile
import shutil
import json
from pprint import pprint

test_name = 'Testing Scoreboard'

def run_tests():
    # Cleanup
    fhqtest.print_bold("Clean up test data... ")
    s = fhqtest.admin_session.scoreboard({"page": 0, "onpage": 10})
    print(s)
