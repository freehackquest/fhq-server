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
Test server api leaks
"""

import uuid
# import pytest


def test_cleanup_leaks_list(admin_session):
    """Cleanup leaks list"""
    print(test_cleanup_leaks_list.__doc__)
    print(admin_session)
    leaks = admin_session.leaks.list({"page": 0, "onpage": 10})
    for leak in leaks["data"]:
        admin_session.leaks.delete({"id": leak["id"]})
        print("Leak with id #" + str(leak["id"]) + " was succesfully deleted!")


def test_leak_creation(admin_session, game1_uuid):
    """Create item leak test"""
    print(test_leak_creation.__doc__)
    leak_uuid1 = uuid.uuid1()
    leak1 = admin_session.leaks.add({
        "uuid": str(leak_uuid1),
        "game_uuid": game1_uuid,
        # "gameid": 0,
        "name": "test_leak1",
        "content": "some_secret_content",
        "score": 100
    })
    print(leak1)
    # Could not get response
    assert leak1 is not None
    assert leak1['result'] == 'DONE'
    assert leak1['data'] is not None
    assert leak1['data']['game_uuid'] == game1_uuid
    assert leak1['data']['name'] == "test_leak1"
    assert leak1['data']['content'] == "some_secret_content"
    assert leak1['data']['score'] == 100
    assert leak1['data']['score'] != 102
    assert leak1['data']['uuid'] == str(leak_uuid1)


def test_leak_update(admin_session):
    """Update item of leak"""
    print(test_leak_update.__doc__)
    leaks = admin_session.leaks.list({"page": 0, "onpage": 10})
    leak_id = None
    assert leaks is not None
    for leak in leaks["data"]:
        assert leak["id"] != ''
        leak_id = leak["id"]
    assert leak_id is not None
    leak_updt = admin_session.leaks.update({
        "id": leak_id,
        "name": "test_leak1_new",
    })
    assert leak_updt is not None
    assert leak_updt['result'] == 'DONE'
    leaks = admin_session.leaks.list({"page": 0, "onpage": 10})
    assert leaks is not None
    for leak in leaks["data"]:
        if leak["id"] == leak_id:
            assert leak["name"] == 'test_leak1_new'
