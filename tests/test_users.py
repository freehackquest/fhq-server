#!/usr/bin/env python
# -*- coding: utf-8 -*-

import libfhqcli
import fhqtest
import sys, traceback
import base64
import json
from pprint import pprint

test_name = 'Testing Users'

# scoreboard
# users
# user
# user_change_password
# user_create
# user_delete
# user_skills
# user_update
user2_session = None
try:
    fhqtest.print_header(" > > > " + test_name + ": begin ")
    fhqtest.init_enviroment()

    # Cleanup
    user1 = None
    user2 = None
    user3 = None
    user3_test = None

    fhqtest.print_bold("Clean up test data... ")
    users = fhqtest.admin_session.users({"onpage": 10, "page": 0, "filter_text": "user1"})
    pprint(users)
    users_count = users["count"]
    for usr in users["data"]:
        if usr['email'] == "user1":
            user1 = usr
        if usr['created'] == '':
            fhqtest.log_err("Field created could not be empty for user #" + str(usr['id']))

    users = fhqtest.admin_session.users({"onpage": 10, "page": 0, "filter_text": "user2"})
    users_count = users["count"]
    for usr in users["data"]:
        if usr['email'] == "user2":
            user2 = usr
        if usr['created'] == '':
            fhqtest.log_err("Field created could not be empty for user #" + str(usr['id']))

    users = fhqtest.admin_session.users({"onpage": 10, "page": 0, "filter_text": "user3"})
    users_count = users["count"]
    for usr in users["data"]:
        if usr['email'] == "user3":
            user3 = usr
        if usr['created'] == '':
            fhqtest.log_err("Field created could not be empty for user #" + str(usr['id']))

    users = fhqtest.admin_session.users({"onpage": 10, "page": 0, "filter_text": "user3_test"})
    users_count = users["count"]
    for usr in users["data"]:
        if usr['email'] == "user3_test":
            user3_test = usr
        if usr['created'] == '':
            fhqtest.log_err("Field created could not be empty for user #" + str(usr['id']))

    if user1 != None:
        fhqtest.admin_session.user_delete({"userid": user1['id'], "password": fhqtest.ADMIN_PASSWORD})
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

    fhqtest.print_bold("Check unique user by email...")
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

    user2_update = fhqtest.admin_session.user_update({"userid": user_found["id"], "country": "Some country"})
    fhqtest.alert(user2_update == None, 'Could not find user2')
    fhqtest.check_response(user2_update, "User2 succesfull updated")
    user2_update = user2_update["data"]
    if "country" not in user2_update:
        fhqtest.log_err("Not found field 'country' in response after user update")
    elif user2_update["country"] != "Some country":
        fhqtest.log_err("Field 'country' expected value of 'Some country'")
    if "university" not in user2_update:
        fhqtest.log_err("Not found field 'university' in response after user update")
    if "nick" not in user2_update:
        fhqtest.log_err("Not found field 'nick' in response after user update")
    if "about" not in user2_update:
        fhqtest.log_err("Not found field 'about' in response after user update")
    if "created" not in user2_update:
        fhqtest.log_err("Not found field 'created' in response after user update")
    if "region" not in user2_update:
        fhqtest.log_err("Not found field 'region' in response after user update")
    if "email" not in user2_update:
        fhqtest.log_err("Not found field 'email' in response after user update")
    if "role" not in user2_update:
        fhqtest.log_err("Not found field 'role' in response after user update")
    if "updated" not in user2_update:
        fhqtest.log_err("Not found field 'updated' in response after user update")
    if "uuid" not in user2_update:
        fhqtest.log_err("Not found field 'uuid' in response after user update")

    fhqtest.print_bold("Login by user2... ")
    user2_session = libfhqcli.FHQCli(fhqtest.TEST_SERVER)
    user2_login = user2_session.login({"email": "user2", "password": "user2"})
    fhqtest.alert(user2_login == None, 'Could not login as user2')
    fhqtest.alert(user2_login['result'] == 'FAIL', 'Could not login as user2 (fail)')
    if user2_login['result'] == 'DONE':
        fhqtest.print_success("User2 login success - OK")
        user2_session.close()
        user2_session = None

    # test user_change_password

    # test user login by different password

    # will not test user_reset_password
    # test login
    # test logout
    # test user_skills
    # test scoreboard

except Exception as e:
    fhqtest.log_err(str(e))
    traceback.print_exc(file=sys.stdout)
finally:
    fhqtest.print_header(" < < < " + test_name + ": end ")
    fhqtest.deinit_enviroment()
    if user2_session != None:
        user2_session.close()
