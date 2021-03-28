#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import traceback
import base64
import os
import zipfile
import shutil
import json
from pprint import pprint
from libfreehackquestclient import FreeHackQuestClient
import fhqtest

TEST_NAME = 'ServerSettings'

def run_tests():
    fhqtest.print_bold("Try update readonly sett...")
    resp = fhqtest.admin_session.server_settings_update({"name": 'app_version', "value": '0.0.3'})
    fhqtest.alert(resp['result'] != 'FAIL', 'Wrong response (server_settings_update app_version)')
    _sett_name = 'mail_signature_text'
    fhqtest.print_bold("Try get '" + _sett_name + "' sett...")
    server_settings = fhqtest.admin_session.server_settings({})
    fhqtest.alert(server_settings is None, 'Could not get response (server_settings)')
    fhqtest.check_response(server_settings, "problem with server_settings")
    # sett = None
    # for _sett in server_settings['data']:
    #     if _sett['name'] == _sett_name:
    #         sett = _sett
    # _oldValue = sett['value']
    _new_value = fhqtest.generate_random(1024).strip()
    fhqtest.print_bold("Try update '" + _sett_name + "' sett...")
    resp = fhqtest.admin_session.server_settings_update({"name": _sett_name, "value": _new_value})
    fhqtest.alert(resp is None, 'Could not get response (server_settings_update)')
    fhqtest.check_response(resp, "request server_settings_update")
    # pprint(resp)

    # request again all the list
    fhqtest.print_bold("Try get again '" + _sett_name + "' sett...")
    server_settings = fhqtest.admin_session.server_settings({})
    fhqtest.alert(server_settings is None, 'Could not get response (server_settings)')
    fhqtest.check_response(server_settings, "request server_settings")
    sett_updated = None
    for _sett in server_settings['data']:
        if _sett['name'] == _sett_name:
            sett_updated = _sett
    _updated_value = sett_updated['value']

    if _updated_value != _new_value:
        fhqtest.throw_err("Expected '" + _new_value + "', but got '" + _updated_value + "'")
    fhqtest.log_ok("Updated settings success")
