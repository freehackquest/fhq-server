# -*- coding: utf-8 -*-
import fhqtest
import libfhqcli
from pprint import pprint
import requests

test_name = 'Testing WebServer'

def run_tests():
    fhqtest.print_header(" > > > " + test_name + ": begin ")

    fhqtest.print_bold("Request main admin page... ")
    r = requests.get('http://localhost:7080/admin/')
    if r.status_code != 200:
        fhqtest.log_err("Wrong sattus code expected 200, but got " + str(r.status_code))
        exit(-1)
    