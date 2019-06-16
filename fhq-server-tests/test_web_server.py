#!/usr/bin/env python
# -*- coding: utf-8 -*-

import libfhqcli
import fhqtest
import sys, traceback
import base64
import json
import requests
from pprint import pprint

test_name = 'Testing Web Server'

user2_session = None
try:
    fhqtest.print_header(" > > > " + test_name + ": begin ")

    fhqtest.print_bold("Request main admin page... ")
    r = requests.get('http://localhost:7080/admin/')
    if r.status_code != 200:
        fhqtest.log_err("Wrong sattus code expected 200, but got " + str(r.status_code))
        exit(-1)

finally:
    fhqtest.print_header(" < < < " + test_name + ": end ")
    
