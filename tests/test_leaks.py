#!/usr/bin/env python
# -*- coding: utf-8 -*-

import libfhqcli
import fhqtest

fhqtest.init_enviroment();

leak_uuid1 = "00000000-0000-0000-0000-000000000000";

# print(fhqtest.game)
print("Try create leak...")
leak1 = fhqtest.admin_session.leaks_add({
    "gameid": 0,
    "game_uuid": fhqtest.game_uuid,
    "uuid": fhqtest.game_uuid,
    "name": "test_leak1",
    "content": "some_secret_content",
    "score": 100
})

fhqtest.alert(leak1 == None, 'Could not get response (2)');

if leak1['result'] == 'FAIL':
    print(leak1['error'])
else:
    print("Leak succesfull created")

fhqtest.deinit_enviroment();
