#!/usr/bin/env python
# -*- coding: utf-8 -*-

import libfhqcli
import fhqtest
import sys, traceback
import base64

test_name = 'Testing Games'

try:
    fhqtest.print_header(" > > > " + test_name + ": begin ");
    fhqtest.init_enviroment();

    # Cleanup
    fhqtest.print_bold("Clean up test data... ")
    s = fhqtest.admin_session.scoreboard({"page": 0, "onpage": 10});
    print(s)

except Exception as e:
    fhqtest.log_err(str(e));
    traceback.print_exc(file=sys.stdout)
finally:
    fhqtest.print_header(" < < < " + test_name + ": end ");
    fhqtest.deinit_enviroment();
