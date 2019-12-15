# -*- coding: utf-8 -*-
import fhqtest
from libfreehackquestclient import FreeHackQuestClient
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
    user_session = FreeHackQuestClient(fhqtest.TEST_SERVER)
    user_login = user_session.login({"email": user, "password": user})
    fhqtest.alert(user_login == None, 'Could not login as ' + user)
    fhqtest.alert(user_login['result'] == 'FAIL', 'Could not login as ' + user + ' (fail)')
    if user_login['result'] == 'DONE':
        fhqtest.print_success("'" + user + "' login success - OK")
        user_session.close()
        user_session = None


def send_msg(session, req):
    res = session.chats_message_send(req)

    pprint(res)

    fhqtest.check_response(res, "succesfull sended")

    fhqtest.alert(res['message'] != req['message'], '"message" not equal')
    fhqtest.alert('user_id' not in res, 'Not member "user_id"')
    fhqtest.alert('dt' not in res, 'Not member "dt"')
    fhqtest.alert(res['dt'] == '', '"dt" empty')
    fhqtest.alert('message_id' not in res, 'Not member "message_id"')
    fhqtest.alert('status' not in res, 'Not member "status"')

    return res


def read_msg(session, req):
    res = session.chats_message_read(req)

    pprint(res)

    fhqtest.check_response(res, "succesfull readed")

    fhqtest.alert('messages' not in res, 'Not member "messages"')
    fhqtest.alert(res['chat'] != req['chat'], '"chat" not equal')
    fhqtest.alert(len(res['messages']) < 1, '"messages" empty')
    fhqtest.alert('message' not in res['messages'][0], 'Not member "messages" in list "messages"')
    fhqtest.alert('user' not in res['messages'][0], 'Not member "user" in list "messages"')
    fhqtest.alert('dt' not in res['messages'][0], 'Not member "dt" in list "messages"')

    return res


def edit_msg(session, req):
    res = session.chats_message_edit(req)

    pprint(res)

    fhqtest.check_response(res, "succesfull edited")

    fhqtest.alert('user_id' not in res, 'Not member "user_id"')
    fhqtest.alert('message_id' not in res, 'Not member "message_id"')

    return res


def delete_msg(session, req):
    res = session.chats_message_delete(req)

    pprint(res)

    fhqtest.check_response(res, "succesfull deleted")

    fhqtest.alert('status' not in res, 'Not member "status"')

    return res


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


    fhqtest.print_bold("Login " + chat_user1_data['email'] + "...")
    chat_user1_session = FreeHackQuestClient(fhqtest.TEST_SERVER)
    chat_user1_login = chat_user1_session.login({
        "email": chat_user1_data['email'], 
        "password": chat_user1_data['password']
    })
    fhqtest.check_response(chat_user1_login, chat_user1_data['nick'] + " succesfull loggined")


    fhqtest.print_bold("Login " + chat_user2_data['email'] + "...")
    chat_user2_session = FreeHackQuestClient(fhqtest.TEST_SERVER)
    chat_user2_login = chat_user2_session.login({
        "email": chat_user2_data['email'], 
        "password": chat_user2_data['password']
    })
    fhqtest.check_response(chat_user2_login, chat_user2_data['nick'] + " succesfull loggined")

    fhqtest.print_bold("Login " + chat_user3_data['email'] + "...")
    chat_user3_session = FreeHackQuestClient(fhqtest.TEST_SERVER)
    chat_user3_login = chat_user3_session.login({
        "email": chat_user3_data['email'], 
        "password": chat_user3_data['password']
    })
    fhqtest.check_response(chat_user3_login, chat_user3_data['nick'] + " succesfull loggined")

    #   send + read

    send_out_1 = send_msg(chat_user1_session, {
        "message": "hello",
        "chat": "0"
    })

    read_out_2 = read_msg(chat_user2_session, {
        "chat": "0"
    })

    read_out_3 = read_msg(chat_user3_session, {
        "chat": "0"
    })

    fhqtest.check_values("double reading", read_out_3['messages'], read_out_2['messages'])

    fhqtest.check_values("sended message", read_out_3['messages'][-1]['message'], send_out_1['message'])
    fhqtest.check_values("sended message", read_out_2['messages'][-1]['message'], send_out_1['message'])

    #   send  + read + edit + read

    send_out_2 = send_msg(chat_user2_session, {
        "message": "ho",
        "chat": "0"
    })

    read_out_3 = read_msg(chat_user3_session, {
        "chat": "0"
    })

    edit_out_2 = edit_msg(chat_user2_session, {
        "message_id": send_out_2["message_id"],
        "message_new": "hi"
    })

    fhqtest.alert(read_out_3['messages'][-1]['message'] == edit_out_2['message'], 'Equal edited message')

    read_out_1 = read_msg(chat_user1_session, {
        "chat": "0"
    })

    fhqtest.check_values("edited message", read_out_1['messages'][-1]['message'], edit_out_2['message'])

    #   delete + read

    cnt_befor = len(read_out_1['messages'])

    delete_out_2 = delete_msg(chat_user2_session, {
        "message_id": send_out_2["message_id"]
    })

    fhqtest.check_values("Deleting message", delete_out_2['status'], "ok")

    read_out_1 = read_msg(chat_user1_session, {
        "chat": "0"
    })

    fhqtest.check_values("Count message after delete", len(read_out_1['messages']), cnt_befor - 1)
