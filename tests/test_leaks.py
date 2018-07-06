#!/usr/bin/env python
# -*- coding: utf-8 -*-

import libfhqcli
import fhqtest

fhqtest.init_enviroment();

print(fhqtest.game)
leak1 = fhqtest.admin_session.leaks_add({
    "game_uuid": fhqtest.game_uuid,
    "name": "test_leak1",
    "content": "some_secret_content",
    "score": 100
})

fhqtest.deinit_enviroment();