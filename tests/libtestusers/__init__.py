# -*- coding: utf-8 -*-
import fhqtest
import libfhqcli

USER1_UUID = "00000000-0000-0000-4001-000000000001"
USER2_UUID = "00000000-0000-0000-4001-000000000002"
USER3_UUID = "00000000-0000-0000-4001-000000000003"

test_name = ' > > > Testing Users'
user1_data = {
    "uuid": USER1_UUID,
    "email": "user1",
    "nick": "nick_user1",
    "password": "user1",
    "role": "user"
}

user2_data = {
    "uuid": USER2_UUID,
    "email": "user2",
    "nick": "nick_user2",
    "password": "user2",
    "role": "user"
}

user3_data = {
    "uuid": USER3_UUID,
    "email": "user3",
    "nick": "nick_user3",
    "password": "user3",
    "role": "user"
}

user3_data_again = {
    "uuid": USER3_UUID,
    "email": "user3",
    "nick": "nick_user3_again",
    "password": "user3",
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