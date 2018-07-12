#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import libfhqcli

admin_session = None
loggined = False
game_uuid = "00000000-0000-0000-0000-000000000000"
game = None;
admin_email = "admin"
admin_password = "admin"

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def print_header(msg):
    global bcolors
    print(bcolors.HEADER + "OK: " + msg + bcolors.ENDC)

def print_success(msg):
    global bcolors
    print(bcolors.OKGREEN + "OK: " + msg + bcolors.ENDC)

def print_bold(msg):
    global bcolors
    print(bcolors.BOLD + msg + bcolors.ENDC)

def log_warn(msg):
    global bcolors
    print(bcolors.WARNING + "Warning: " + msg + bcolors.ENDC + "\n")

def log_err(msg):
    global bcolors
    print(bcolors.FAIL + "Error: " + msg + bcolors.ENDC + "\n")

def alert(check, msg):
    if(check == True):
        log_err(msg);
        raise Exception(msg);
        sys.exit(0)

def check_response(resp, msg_success):
    if(resp['result'] == 'FAIL'):
        log_err(resp['error']);
    else:
        print_success(msg_success)

def init_enviroment():
    global admin_session
    global game
    global game_uuid
    global admin_email
    global admin_password
    
    admin_session = libfhqcli.FHQCli("ws://localhost:1234/");
    resp = admin_session.login({"email": admin_email, "password": admin_password});
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
