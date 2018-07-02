#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import libfhqcli

admin_session = None;
game = None;

def alert(check, msg):
    if(check == True):
        print("Error: " + msg)
        sys.exit(0)

def init_enviroment():
    global admin_session
    global game
    admin_session = libfhqcli.FHQCli("ws://localhost:1234/");
    resp = admin_session.login({"email": "admin", "password": "admin"});
    alert(resp == None, 'Could not login as admin (1)');
    alert(resp['result'] == 'FAIL', 'Could not login as admin (2)');

    # admin_session.game_create()
    resp = admin_session.game_create({"uuid": "", "name": "", "description": "", "state": "", "form": "", "type": "", "date_start": "", "date_stop": "", "date_restart": "", "organizators": ""});
    alert(resp == None, 'Could not send message (2)');
    alert(resp['result'] == 'FAIL', 'Could not create test game');

def deinit_enviroment():
    global admin_session
    global game

    admin_session.close();
