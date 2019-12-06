#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from libfreehackquestclient import FreeHackQuestClient
import fhqtest
import sys, traceback
import base64
import json
from pprint import pprint
import random

test_name = 'Testing Knowledge Base'

def run_tests():

    fhqtest.clean_all_classbooks()

    records = []
    for _ in range(10):
        records.append({
            'name': fhqtest.generate_random(10),
            'content': fhqtest.generate_random(255),
            'uuid': fhqtest.generate_random_uuid(),
        })

    fhqtest.print_bold("Create new records")
    parentids = []
    parentids.append(0) # root
    for rec in records:
        rec['parentid'] = random.choice(parentids)
        r = fhqtest.admin_session.classbook_add_record({
            "parentid": rec['parentid'],
            "name": rec['name'],
            "content": rec['content'],
            "uuid": rec['uuid'],
            "ordered": 0
        })
        fhqtest.check_response(r, "Record " + rec['uuid'] + " succesfull in parent " + str(rec['parentid']) + " created")
        if r['result'] == 'FAIL':
            raise ValueError('Could not create new classbook')

        rec['classbookid'] = r['data']['classbookid']
        parentids.append(rec['classbookid'])

    # pprint(records)
    # Cleanup
    ''' user1 = None
    user2 = None
    user3 = None


    fhqtest.print_bold("Clean up test data... ")
    users = fhqtest.admin_session.users({"onpage": 10, "page": 0, "filter_text": "user1"})
    users_count = users["count"]
    for usr in users["data"]:
        if usr['email'] == "user1":
            user1 = usr
        if usr['created'] == '':
            fhqtest.log_err("Field created could not be empty for user #" + str(usr['id']))
            exit(-1)

    users = fhqtest.admin_session.users({"onpage": 10, "page": 0, "filter_text": "user2"})
    users_count = users["count"]
    for usr in users["data"]:
        if usr['email'] == "user2":
            user2 = usr
        if usr['created'] == '':
            fhqtest.log_err("Field created could not be empty for user #" + str(usr['id']))
            exit(-1)

    users = fhqtest.admin_session.users({"onpage": 10, "page": 0, "filter_text": "user3"})
    users_count = users["count"]
    for usr in users["data"]:
        if usr['email'] == "user3":
            user3 = usr
        if usr['created'] == '':
            fhqtest.log_err("Field created could not be empty for user #" + str(usr['id']))
            exit(-1)

    users = fhqtest.admin_session.users({"onpage": 10, "page": 0, "filter_text": "user3_test"})
    users_count = users["count"]
    for usr in users["data"]:
        if usr['email'] == "user3_test":
            user3_test = usr
        if usr['created'] == '':
            fhqtest.log_err("Field created could not be empty for user #" + str(usr['id']))
            exit(-1)

    if user1 != None:
        user1_delete = fhqtest.admin_session.user_delete({
            "userid": user1['id'], 
            "password": fhqtest.ADMIN_PASSWORD
        })
        fhqtest.check_response(user1_delete, "User1 succesfull removed")
        user1 = None

    if user2 != None:
        fhqtest.admin_session.user_delete({"userid": user2['id'], "password": fhqtest.ADMIN_PASSWORD})
        user2 = None

    if user3 != None:
        fhqtest.admin_session.user_delete({"userid": user3['id'], "password": fhqtest.ADMIN_PASSWORD})
        user3 = None

    if user3_test != None:
        fhqtest.admin_session.user_delete({"userid": user3_test['id'], "password": fhqtest.ADMIN_PASSWORD})
        user3_test = None
    fhqtest.print_success("Cleaned")

    # test create users
    fhqtest.print_bold("Try create user1...")
    user1 = fhqtest.admin_session.users_add({
        "uuid": fhqtest.USER1_UUID,
        "email": "user1",
        "nick": "nick_user1",
        "password": "user1",
        "role": "user"
    })

    fhqtest.alert(user1 == None, 'Could not get response (users_add/user1)')
    fhqtest.check_response(user1, "User1 succesfull created")

    fhqtest.print_bold("Try create user2...")
    user2 = fhqtest.admin_session.users_add({
        "uuid": fhqtest.USER2_UUID,
        "email": "user2",
        "nick": "nick_user2",
        "password": "user2",
        "role": "user"
    })

    fhqtest.alert(user2 == None, 'Could not get response (users_add/user2)')
    fhqtest.check_response(user1, "User2 succesfull created")

    fhqtest.print_bold("Try create user3...")
    user3 = fhqtest.admin_session.users_add({
        "uuid": fhqtest.USER3_UUID,
        "email": "user3",
        "nick": "nick_user3",
        "password": "user3",
        "role": "user"
    })

    fhqtest.alert(user3 == None, 'Could not get response (users_add/user3)');
    fhqtest.check_response(user1, "User3 succesfull created")

    fhqtest.print_bold("Check unique user3 by email...")
    user3_again = fhqtest.admin_session.users_add({
        "uuid": fhqtest.USER3_UUID,
        "email": "user3",
        "nick": "nick_user3_again",
        "password": "user3",
        "role": "user"
    })

    fhqtest.alert(user3_again == None, 'Could not get response (users_add/user3 again)')
    if user3_again['result'] == "DONE":
        fhqtest.log_err("Wrong creating user email must be unique")
    else:
        fhqtest.print_success("User3 again - OK")

    fhqtest.print_bold("Check unique user by uuid...")
    user3_again2 = fhqtest.admin_session.users_add({"uuid": fhqtest.USER3_UUID, "email": "user3_test", "nick": "nick_user3_again", "password": "user3", "role": "user"})
    fhqtest.alert(user3_again2 == None, 'Could not get response (users_add/user3 again2)')
    if user3_again2['result'] == "DONE":
        fhqtest.log_err("Wrong creating user uuid must be unique")
    else:
        fhqtest.print_success("User3 again - OK")

    fhqtest.print_bold("Find user by nick...")
    users = fhqtest.admin_session.users({"onpage": 10, "page": 0, "filter_text": "nick_user2"})
    users_count = users["count"]
    user_found = None
    for usr in users["data"]:
        if usr['email'] == "user2":
            user_found = usr
    fhqtest.alert(user_found == None, 'Could not find user2')
    if user_found != None:
        fhqtest.print_success("User2 found success - OK")

    fhqtest.print_bold("Update user country... ")
    fhqtest.alert(user_found["country"] != "", 'User country must be empty ')

    user2_update = fhqtest.admin_session.user_update({
        "userid": user_found["id"],
        "country": "Some country",
        "university": "Some university",
    })
    fhqtest.alert(user2_update == None, 'Could not find user2')
    fhqtest.check_response(user2_update, "User2 succesfull updated")
    user2_update = user2_update["data"]
    if "country" not in user2_update:
        fhqtest.log_err("Not found field 'country' in response after user update")
        exit(-1)
    elif user2_update["country"] != "Some country":
        fhqtest.log_err("Field 'country' expected value of 'Some country'")
        exit(-1)
    if "university" not in user2_update:
        fhqtest.log_err("Not found field 'university' in response after user update")
        exit(-1)
    elif  user2_update["university"] != "Some university":
        fhqtest.log_err("Field 'university' expected value of 'Some university'")
        exit(-1)
    if "nick" not in user2_update:
        fhqtest.log_err("Not found field 'nick' in response after user update")
        exit(-1)
    if "about" not in user2_update:
        fhqtest.log_err("Not found field 'about' in response after user update")
        exit(-1)
    if "created" not in user2_update:
        fhqtest.log_err("Not found field 'created' in response after user update")
        exit(-1)
    if "region" not in user2_update:
        fhqtest.log_err("Not found field 'region' in response after user update")
        exit(-1)
    if "email" not in user2_update:
        fhqtest.log_err("Not found field 'email' in response after user update")
        exit(-1)
    if "role" not in user2_update:
        fhqtest.log_err("Not found field 'role' in response after user update")
        exit(-1)
    # TODO
    # if "updated" not in user2_update:
    #     fhqtest.log_err("Not found field 'updated' in response after user update")
    #     exit(-1)
    if "uuid" not in user2_update:
        fhqtest.log_err("Not found field 'uuid' in response after user update")
        exit(-1)

    fhqtest.print_bold("Login by user2... ")
    user2_session = FreeHackQuestClient(fhqtest.TEST_SERVER)
    user2_login = user2_session.login({"email": "user2", "password": "user2"})
    fhqtest.alert(user2_login == None, 'Could not login as user2')
    fhqtest.alert(user2_login['result'] == 'FAIL', 'Could not login as user2 (fail)')
    if user2_login['result'] == 'DONE':
        fhqtest.print_success("User2 login success - OK")
        user2_session.close()
        user2_session = None


    # deprecated method
    user2_found1 = fhqtest.admin_session.user({"userid": user_found["id"]})
    fhqtest.alert(user2_found1 == None, 'Could not get user2')
    fhqtest.check_response(user2_found1, "User2 succesfull got (1)")
    user2_found1 = user2_found1['data']
    if "email" not in user2_found1:
        fhqtest.log_err("Not found field 'email' in response user2_found1")
        exit(-1)
    elif user2_found1['email'] != 'user2':
        fhqtest.log_err("Expected field 'email' value 'user2', but got '" + user2_found1['email'] + "'")
        exit(-1)

    # TODO
    user2_found2 = fhqtest.admin_session.users_info({"uuid": user_found["id"]})
    fhqtest.alert(user2_found2 == None, 'Could not get user2')
    fhqtest.check_response(user2_found2, "User2 succesfull got (2)")
    if "email" not in user2_found1:
        fhqtest.log_err("Not found field 'email' in response user2_found1")
        exit(-1)
    elif user2_found1['email'] != 'user2':
        fhqtest.log_err("Expected field 'email' value 'user2', but got '" + user2_found1['email'] + "'")
        exit(-1)
    '''
    fhqtest.clean_all_classbooks()
