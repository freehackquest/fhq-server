#!/usr/bin/env python3
# Copyright (C) 2011-2025, Evgenii Sopov <mrseakg@gmail.com>

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
