#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from libfreehackquestclient import FreeHackQuestClient
import fhqtest
import sys, traceback
import base64
import os
import zipfile
import shutil
import json
from pprint import pprint
import libtestusefullinks

test_name = 'Testing Useful Links'

def run_tests():
   
    # Cleanup
    fhqtest.print_bold("Clean up test data... ")
    ul_list = fhqtest.admin_session.useful_links_list({ "filter": "" })
    fhqtest.alert(ul_list == None, 'Could not get response')
    fhqtest.print_success("Cleaned")

    url1 = fhqtest.generate_random(2048)
    description1 = fhqtest.generate_random(2048)
    author1 = fhqtest.generate_random(127)

    # test create game
    fhqtest.print_bold("Try create useful_link...")
    ul_add = fhqtest.admin_session.useful_links_add({
        "url": url1,
        "description": description1,
        "author": author1,
    })
    pprint(ul_add)
    fhqtest.alert(ul_add == None, 'Could not get response (2)')
    fhqtest.check_response(ul_add, "Useful Link succesfull created")

    if ul_add['result'] == 'FAIL':
        fhqtest.throw_err('Could not create Useful Link')


    
