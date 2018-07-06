#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import libfhqcli

admin_session = None
loggined = False
game_uuid = "00000000-0000-0000-0000-000000000000"
game = None;

def alert(check, msg):
    if(check == True):
        print("Error: " + msg)
        sys.exit(0)

def init_enviroment():
    global admin_session
    global game
    global game_uuid
    admin_session = libfhqcli.FHQCli("ws://localhost:1234/");
    resp = admin_session.login({"email": "admin", "password": "admin"});
    alert(resp == None, 'Could not login as admin (1)');
    alert(resp['result'] == 'FAIL', 'Could not login as admin (2)');
    
    loggined = True
    game = admin_session.game_info({"uuid": game_uuid})
    alert(resp == None, 'Could not get test game (2)');
    if game['result'] == 'FAIL':
        game = admin_session.game_create({
            "uuid": game_uuid,
            "name": "test",
            "description": "test",
            "state": "",
            "form": "",
            "type": "",
            "date_start": "2000-01-01 00:00:00",
            "date_stop": "2001-01-01 00:00:00",
            "date_restart": "2002-01-01 00:00:00",
            "organizators": "test"
        });
        alert(game == None, 'Could not send message (2)');
        alert(game['result'] == 'FAIL', 'Could not create test game ' + str(resp));
        game = admin_session.game_info({"uuid": game_uuid})

    game = game['data']

def deinit_enviroment():
    global admin_session
    global game
    global game_uuid

    # try remove all test objects
    if loggined == True:
        admin_session.game_delete({"uuid": game_uuid, "admin_password": "admin"})

    admin_session.close();
