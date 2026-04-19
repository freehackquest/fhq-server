#!/usr/bin/env python3
##################################################################################
#                      Project
#    __ _
#   / _| |__   __ _       ___  ___ _ ____   _____ _ __
#  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
#  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
#  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
#                |_|
#
# MIT License
#
# Copyright (c) 2011-2026 FreeHackQuest <freehackquest@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Original repository: https://github.com/freehackquest/fhq-server
#
##################################################################################

"""
Test Server Settings
"""

import random
import string


def generate_random(size):
    """Generate random printable string"""
    _range = range(size)
    _alphabet = string.ascii_uppercase + string.digits + ' _+=\'"~@!#?/<>'
    return ''.join(random.choice(_alphabet) for _ in _range)


def test_server_settings_readonly(admin_session):
    """ Server settings readonly """
    print(test_server_settings_readonly.__doc__)
    server_api = admin_session.server_settings_update({"name": 'app_version', "value": '0.0.3'})
    assert server_api is not None
    assert server_api["result"] == "FAIL"
    print(server_api)


def test_server_settings_update(admin_session):
    """ Server settings """
    print(test_server_settings_update.__doc__)
    server_settings = admin_session.server_settings({})
    assert server_settings is not None
    assert server_settings["result"] == "DONE"

    # print(server_settings)
    _sett_name = 'mail_signature_text'
    _new_value = generate_random(1024).strip()
    _old_sett = None
    for _sett in server_settings['data']:
        if _sett['name'] == _sett_name:
            _old_sett = _sett
    print(_old_sett)
    sett_updated = admin_session.server_settings_update({
        "name": _sett_name,
        "value": _new_value
    })
    assert sett_updated is not None
    assert sett_updated["result"] == "DONE"
    server_settings2 = admin_session.server_settings({})
    assert server_settings2 is not None
    assert server_settings2["result"] == "DONE"
    _new_sett = None
    for _sett in server_settings2['data']:
        if _sett['name'] == _sett_name:
            _new_sett = _sett
    print(_new_sett)
    assert _new_sett['value'] != _old_sett['value']
    assert _new_sett['value'] == _new_value


def test_server_uuid_generate(admin_session):
    """ Server uuid generate """
    print(test_server_settings_readonly.__doc__)
    ret = admin_session.server.uuid_generate({"typeobj": 'test'})
    print(ret)
    assert ret is not None
    assert ret["result"] is not None
    assert ret["result"]["typeobj"] == "test"


def test_server_uuid_info(admin_session):
    """ Server uuid info """
    print(test_server_settings_readonly.__doc__)
    ret = admin_session.server.uuid_generate({"typeobj": 'test'})

    assert ret is not None
    assert ret["result"] is not None
    assert ret["result"]["typeobj"] == "test"
    _uuid = ret["result"]["uuid"]

    ret = admin_session.server.uuid_info({"uuid": _uuid})
    assert ret is not None
    assert ret["result"] is not None
    assert ret["result"]["typeobj"] == "test"
    assert ret["result"]["uuid"] == _uuid

    ret = admin_session.server.uuid_info({"uuid": "00000000-0000-0000-0000-000000000000"})
    assert ret is not None
    assert ret['error'] is not None
    assert ret['error']['code'] == 404
