# -*- coding: utf-8 -*-
import fhqtest
from pprint import pprint
import requests

test_name = 'Testing WebServer'

def run_tests():
    fhqtest.print_header(" > > > " + test_name + ": begin ")
    fhqtest.print_bold("Request main admin page... ")
    r = requests.get(fhqtest.TEST_WEB_SERVER + 'admin/')
    if r.status_code != 200:
        fhqtest.log_err("Wrong status code expected 200, but got " + str(r.status_code))
        exit(-1)
    fhqtest.log_ok("admin page success got")


    