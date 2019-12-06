#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
from libfreehackquestclient import FreeHackQuestClient
import string
import random
import socket
from pprint import pprint

admin_session = None
user_session = None
anon_session = None
loggined = False
GAME_UUID1 = "00000000-0000-0000-1000-000000000001"
GAME_UUID2 = "00000000-0000-0000-1000-000000000002"
CLASSBOOK_RECORD1 = "C1A55800-0000-0000-0000-000000000001"
CLASSBOOK_RECORD2 = "C1A55800-0000-0000-0000-000000000002"
CLASSBOOK_RECORD3 = "C1A55800-0000-0000-0000-000000000003"
CLASSBOOK_RECORD4 = "C1A55800-0000-0000-0000-000000000004"
QUEST_UUID1 = "03E51000-0000-0000-0000-000000000001"
QUEST_UUID2 = "03E51000-0000-0000-0000-000000000002"
QUEST_UUID3 = "03E51000-0000-0000-0000-000000000003"
QUEST_UUID4 = "03E51000-0000-0000-0000-000000000004"
GAME1 = None
admin_email = "admin" # deprecated
admin_password = "admin" # deprecated
ADMIN_EMAIL = "admin"
ADMIN_PASSWORD = "admin"
TEST_HOST = "127.0.0.1"
TEST_SERVER = "ws://" + TEST_HOST + ":1234/"
TEST_WEB_SERVER = "http://" + TEST_HOST + ":7080/"
TMP_DIR = "./tmp"

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
    print(bcolors.WARNING + "Warning: " + msg + bcolors.ENDC)

def log_err(msg):
    global bcolors
    print(bcolors.FAIL + "ERROR: " + msg + bcolors.ENDC)

def log_ok(msg):
    global bcolors
    print(bcolors.OKGREEN + "OK: " + msg + bcolors.ENDC)

def throw_err(msg):
    global bcolors
    print(bcolors.FAIL + "ERROR: " + msg + bcolors.ENDC)
    raise Exception(msg)

def alert(check, msg):
    if (check == True):
        throw_err(msg)

def check_port(host, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    result = sock.connect_ex((host,port))
    return result == 0

def generate_random(size): 
    return ''.join(random.choice(string.ascii_uppercase + string.digits + ' _+=\'"~@!#?/<>') for _ in range(size))

def generate_random_uuid(): 
    ret = ''
    ret = ret + ''.join(random.choice('0123456789abcdef') for _ in range(8))
    ret = ret + '-'
    ret = ret + ''.join(random.choice('0123456789abcdef') for _ in range(4))
    ret = ret + '-'
    ret = ret + ''.join(random.choice('0123456789abcdef') for _ in range(4))
    ret = ret + '-'
    ret = ret + ''.join(random.choice('0123456789abcdef') for _ in range(4))
    ret = ret + '-'
    ret = ret + ''.join(random.choice('0123456789abcdef') for _ in range(12))
    return ret

def check_response(resp, msg_success):
    alert(resp == None, 'Wrong response')
    if resp['result'] == 'FAIL':
        log_err(resp['error'])
    else:
        print_success(msg_success)

def check_values(what, got, expected):
    print_bold("Check the " + what + "... ")
    if got != expected:
        throw_err(what + " has wrong expected '" + str(expected) + "' got '" + str(got) + "'")
    else:
        print_success(what + " is ok")

def init_enviroment():
    global admin_session
    global ADMIN_EMAIL
    global ADMIN_PASSWORD
    global TEST_SERVER
    
    if not os.path.exists(TMP_DIR):
        os.makedirs(TMP_DIR)

    admin_session = FreeHackQuestClient(TEST_SERVER)
    resp = admin_session.login({"email": ADMIN_EMAIL, "password": ADMIN_PASSWORD})
    alert(resp == None, 'Could not login as admin (1)')
    # pprint(resp)
    alert(resp['result'] == 'FAIL', 'Could not login as admin (2)')
    
    # loggined = True
    GAME1 = admin_session.game_info({"uuid": GAME_UUID1})
    alert(resp == None, 'Could not get test game (2)')
    if GAME1['result'] == 'FAIL':
        GAME1 = admin_session.game_create({
            "uuid": GAME_UUID1,
            "name": "test",
            "description": "test",
            "state": "original",
            "form": "online",
            "type": "jeopardy",
            "date_start": "2000-01-01 00:00:00",
            "date_stop": "2001-01-01 00:00:00",
            "date_restart": "2002-01-01 00:00:00",
            "organizators": "test"
        })
        alert(GAME1 == None, 'Could not send message (2)')
        alert(GAME1['result'] == 'FAIL', 'Could not create test game ' + str(GAME1))
        GAME1 = admin_session.game_info({"uuid": GAME_UUID1})

    GAME1 = GAME1['data']

def deinit_enviroment():
    global admin_session

    # try remove all test objects
    if loggined == True:
        admin_session.game_delete({"uuid": GAME_UUID1, "admin_password": "admin"})
    if admin_session != None:
        admin_session.close()

def remove_item(classbookid, padding):
    records_list = admin_session.classbook_list({
        "parentid": classbookid,
    })
    childs = len(records_list['data'])
    print("     * " + padding + " " + str(classbookid) + " has " + str(childs) + " childs")
    # check_response(records_list, "Classbook Records list got")
    for clb in records_list['data']:
        childs = childs + 1
        child_classbookid = clb['classbookid']
        remove_item(child_classbookid, padding + "-(" + str(child_classbookid) + " has " + str(clb['childs']) + " childs)-")
    
    if classbookid != 0:
        # print("Try remove classbook record #" + str(child_classbookid))
        r = admin_session.classbook_delete_record({"classbookid": classbookid})
        # check_response(r, "Record #" + str(child_classbookid) + " succesfull removed")
        if r['result'] == 'FAIL':
            pprint(r)
            raise ValueError('Could not remove classbook record #' + str(classbookid))
        else:
            print_success(" * " + padding + "+ " + str(classbookid) + " - removed")

def clean_all_classbooks():
    print_bold("Cleanup all classbook test data... ")
    remove_item(0, "-")

def clean_all_quests():
    print_bold("Cleanup quests test data... ")
    quests = admin_session.quests({})
    alert(quests == None, 'Could not get response')
    quests = quests['data']
    for q in quests:
        questid = q['questid']
        quest_removed = admin_session.quest_delete({"questid": questid})
        check_response(quest_removed, "Quest #" + str(questid) + " succesfull removed")

def find_first_gameid():
    print_bold("Search first game id...")
    games1 = admin_session.games({})
    check_response(games1, "Games succesfull got")
    games1 = games1['data']
    gameid = 0
    if len(games1) == 0:
        log_err("Not found any game")
    return games1[0]['local_id']
