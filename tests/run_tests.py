#!/usr/bin/env python
# -*- coding: utf-8 -*-

import signal
import sys
import libfhq


fhq_admin = libfhq.FHQCli("ws://localhost:1234/");
admin_user = fhq_admin.login("admin", "admin");
print("admin_user: " + str(admin_user));
res = fhq_admin.admin_user_create("user1", "user1", "user1", "user", "...");
res = fhq_admin.admin_user_create("user2", "user2", "user2", "user", "...");
res = fhq_admin.admin_user_create("user3", "user3", "user3", "user", "...");
print(res);
scoreboard = fhq_admin.scoreboard();
print("scoreboard: " + str(scoreboard));

fhq_admin.close();

# TODO KeyboardInterrupt process

'''
def signal_handler(signal, frame):
        print('You pressed Ctrl+C!')
        sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)
print('Press Ctrl+C')
signal.pause()
'''