# -*- coding: utf-8 -*-
import fhqtest
import libfhqcli
from pprint import pprint
import sys
import time

CHAT_USER1_UUID = "00000000-0000-5000-4001-000000000001"
CHAT_USER2_UUID = "00000000-0000-5000-4001-000000000002"
CHAT_USER3_UUID = "00000000-0000-5000-4001-000000000003"

test_name = 'Testing Users'
chat_user1_data = {
    "uuid": CHAT_USER1_UUID,
    "email": "chat_user1",
    "nick": "nick_chat_user1",
    "password": "chat_user1",
    "role": "user"
}

chat_user2_data = {
    "uuid": CHAT_USER2_UUID,
    "email": "chat_user2",
    "nick": "nick_chat_user2",
    "password": "chat_user2",
    "role": "user"
}

chat_user3_data = {
    "uuid": CHAT_USER3_UUID,
    "email": "chat_user3",
    "nick": "nick_chat_user3",
    "password": "chat_user3",
    "role": "user"
}

def cleanup_user_by_email(email):
    users = fhqtest.admin_session.users({"onpage": 10, "page": 0, "filter_text": email })
    user = None
    for usr in users["data"]:
        if usr['email'] == email:
            user = usr
        if usr['created'] == '':
            fhqtest.log_err("Field created could not be empty for user #" + str(usr['id']))
            exit(-1)

    if user != None:
        print("Removing '" + email + "' ...")
        user_delete = fhqtest.admin_session.user_delete({
            "userid": user['id'], 
            "password": fhqtest.ADMIN_PASSWORD
        })
        fhqtest.check_response(user_delete, "User '" + email + "' succesfull removed")

def test_login(user):
    fhqtest.print_bold("Login by '" + user + "'... ")
    user_session = libfhqcli.FHQCli(fhqtest.TEST_SERVER)
    user_login = user_session.login({"email": user, "password": user})
    fhqtest.alert(user_login == None, 'Could not login as ' + user)
    fhqtest.alert(user_login['result'] == 'FAIL', 'Could not login as ' + user + ' (fail)')
    if user_login['result'] == 'DONE':
        fhqtest.print_success("'" + user + "' login success - OK")
        user_session.close()
        user_session = None

def run_tests():
    chat_user1 = None
    chat_user2 = None
    chat_user3 = None

    fhqtest.print_bold("Clean up test data... ")
    cleanup_user_by_email("chat_user1")
    cleanup_user_by_email("chat_user2")
    cleanup_user_by_email("chat_user3")
    fhqtest.print_success("Cleaning complete")

    # test create users
    fhqtest.print_bold("Create " + chat_user1_data['email'] + "...")
    chat_user1 = fhqtest.admin_session.users_add(chat_user1_data)
    fhqtest.check_response(chat_user1, chat_user1_data['nick'] + " succesfull created")

    fhqtest.print_bold("Create " + chat_user2_data['email'] + "...")
    chat_user2 = fhqtest.admin_session.users_add(chat_user2_data)
    fhqtest.check_response(chat_user2, chat_user2_data['nick'] + " succesfull created")

    fhqtest.print_bold("Create " + chat_user3_data['email'] + "...")
    chat_user3 = fhqtest.admin_session.users_add(chat_user3_data)
    fhqtest.check_response(chat_user3, chat_user3_data['nick'] + " succesfull created")

    chat_user1_session = None
    chat_user2_session = None
    chat_user3_session = None

    try:
        fhqtest.print_bold("Login " + chat_user1_data['email'] + "...")
        chat_user1_session = libfhqcli.FHQCli(fhqtest.TEST_SERVER)
        chat_user1_login = chat_user1_session.login({
            "email": chat_user1_data['email'], 
            "password": chat_user1_data['password']
        })
        fhqtest.check_response(chat_user1_login, chat_user1_data['nick'] + " succesfull loggined")


        fhqtest.print_bold("Login " + chat_user2_data['email'] + "...")
        chat_user2_session = libfhqcli.FHQCli(fhqtest.TEST_SERVER)
        chat_user2_login = chat_user2_session.login({
            "email": chat_user2_data['email'], 
            "password": chat_user2_data['password']
        })
        fhqtest.check_response(chat_user2_login, chat_user2_data['nick'] + " succesfull loggined")

        fhqtest.print_bold("Login " + chat_user3_data['email'] + "...")
        chat_user3_session = libfhqcli.FHQCli(fhqtest.TEST_SERVER)
        chat_user3_login = chat_user3_session.login({
            "email": chat_user3_data['email'], 
            "password": chat_user3_data['password']
        })
        fhqtest.check_response(chat_user3_login, chat_user3_data['nick'] + " succesfull loggined")

        chat_user1_session.chat_send_message({
            "type": "chat",
            "message": "hello",
        })
        time.sleep(5)
        latest_messages = chat_user2_session.chat_latest_messages({})
        fhqtest.check_response(latest_messages, "get response")
        msg = latest_messages["data"][0]

        fhqtest.alert(msg['message'] != "hello", 'Expected message "hello"')
        fhqtest.alert(msg['user'] != chat_user2_data['nick'], 'Expected author of message "' + chat_user2_data['nick'] + '"')

        print("Response: ")
        pprint(msg)

    finally:
        if chat_user1_session != None:
            chat_user1_session.close()

        if chat_user2_session != None:
            chat_user2_session.close()

        if chat_user2_session != None:
            chat_user2_session.close()
        sys.exit(-1)

