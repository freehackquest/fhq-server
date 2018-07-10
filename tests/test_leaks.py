#!/usr/bin/env python
# -*- coding: utf-8 -*-

import libfhqcli
import fhqtest
import sys, traceback

fhqtest.print_header(" > > > Testing Leaks: begin ");

try:
    fhqtest.init_enviroment();
    
    leak_uuid1 = "00000000-0000-0000-0000-000000000001";
    fhqtest.print_bold("Try create leak...")
    leak1 = fhqtest.admin_session.leaks_add({
        "gameid": 0,
        "game_uuid": fhqtest.game_uuid,
        "uuid": leak_uuid1,
        "name": "test_leak1",
        "content": "some_secret_content",
        "score": 100
    })
    fhqtest.alert(leak1 == None, 'Could not get response (2)');
    fhqtest.check_response(leak1, "Leak succesfull created")
    
except Exception as e:
    fhqtest.log_err(str(e));
    traceback.print_exc(file=sys.stdout)
finally:
    fhqtest.print_header(" < < < Testing Leaks: end ");
    fhqtest.deinit_enviroment();
