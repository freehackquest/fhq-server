#!/usr/bin/env python
# -*- coding: utf-8 -*-

import signal
import sys
import libfhq

def signal_handler(signal, frame):
        print('You pressed Ctrl+C!')
        sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)
print('For stop test please press Ctrl+C')
# signal.pause()

def alert(check, msg):
    if(check == True):
        print("Error: " + msg)
        sys.exit(0)

try:
    fhq_admin = libfhq.FHQCli("ws://localhost:1234/");
    resp = fhq_admin.login("admin", "admin");
    alert(resp == None, 'Could not login as admin (1)');
    alert(resp['result'] == 'FAIL', 'Could not login as admin (2)');
    print("admin_user: " + str(admin_user));
    res = fhq_admin.admin_user_create("user1", "user1", "user1", "user", "...");
    res = fhq_admin.admin_user_create("user2", "user2", "user2", "user", "...");
    res = fhq_admin.admin_user_create("user3", "user3", "user3", "user", "...");
    print(res);
    scoreboard = fhq_admin.scoreboard();
    print("scoreboard: " + str(scoreboard));
    fhq_admin.close();
except (KeyboardInterrupt, SystemExit):
    fhq_admin.close();
    sys.exit()
    
    '''
fhq_admin = libfhq.FHQCli("ws://localhost:1234/");
resp = fhq_admin.login("admin", "admin");
alert(resp['result'] == 'FAIL', 'Could not login by admin');

print("admin_user: " + str(admin_user));
res = fhq_admin.admin_user_create("user1", "user1", "user1", "user", "...");
res = fhq_admin.admin_user_create("user2", "user2", "user2", "user", "...");
res = fhq_admin.admin_user_create("user3", "user3", "user3", "user", "...");
print(res);
scoreboard = fhq_admin.scoreboard();
print("scoreboard: " + str(scoreboard));

fhq_admin.close();
'''

# TODO KeyboardInterrupt process

'''

'''