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

test_name = 'ServerSettings'

def run_tests():
    fhqtest.print_bold("Try update readonly sett...")
    resp = fhqtest.admin_session.server_settings_update({"name": 'app_version', "value": '0.0.3'})
    fhqtest.alert(resp['result'] != 'FAIL', 'Wrong response (server_settings_update app_version)')
    
    _settName = 'mail_signature_text'
    fhqtest.print_bold("Try get '" + _settName + "' sett...")
    server_settings = fhqtest.admin_session.server_settings({})
    fhqtest.alert(server_settings == None, 'Could not get response (server_settings)')
    fhqtest.check_response(server_settings, "problem with server_settings")
    sett = None
    for _sett in server_settings['data']:
        if _sett['name'] == _settName:
            sett = _sett

    _oldValue = sett['value']
    _newValue = fhqtest.generate_random(1024).strip()
    fhqtest.print_bold("Try update '" + _settName + "' sett...")
    resp = fhqtest.admin_session.server_settings_update({"name": _settName, "value": _newValue})
    fhqtest.alert(resp == None, 'Could not get response (server_settings_update)')
    fhqtest.check_response(resp, "request server_settings_update")
    # pprint(resp)

    # request again all the list
    fhqtest.print_bold("Try get again '" + _settName + "' sett...")
    server_settings = fhqtest.admin_session.server_settings({})
    fhqtest.alert(server_settings == None, 'Could not get response (server_settings)')
    fhqtest.check_response(server_settings, "request server_settings")
    settUpdated = None
    for _sett in server_settings['data']:
        if _sett['name'] == _settName:
            settUpdated = _sett
    _updatedValue = settUpdated['value']

    if _updatedValue != _newValue:
        fhqtest.throw_err("Expected '" + _newValue + "', but got '" + _updatedValue + "'")
    fhqtest.log_ok("Updated settings success")


