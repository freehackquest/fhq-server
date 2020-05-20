# -*- coding: utf-8 -*-
import fhqtest
from libfreehackquestclient import FreeHackQuestClient
from pprint import pprint

USER1_UUID = "00000000-0000-0000-4001-000000000001"
USER2_UUID = "00000000-0000-0000-4001-000000000002"
USER3_UUID = "00000000-0000-0000-4001-000000000003"

test_name = 'Testing Users'
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
    __user_session = FreeHackQuestClient(fhqtest.TEST_SERVER)
    user_login = __user_session.login({"email": user, "password": user})
    fhqtest.alert(user_login == None, 'Could not login as ' + user)
    fhqtest.alert(user_login['result'] == 'FAIL', 'Could not login as ' + user + ' (fail)')
    if user_login['result'] == 'DONE':
        fhqtest.print_success("'" + user + "' login success - OK")
        __user_session.close()
        __user_session = None

def run_tests():
    # scoreboard
    # users
    # user
    # user_change_password
    # user_create
    # user_delete
    # user_skills
    # user_update
    user2_session = None
    # Cleanup
    user1 = None
    user2 = None
    user3 = None
    user3_test = None

    fhqtest.print_bold("Clean up test data... ")
    cleanup_user_by_email("user1")
    cleanup_user_by_email("user2")
    cleanup_user_by_email("user3")
    cleanup_user_by_email("user3_test")
    fhqtest.print_success("Cleaning complete")

    # test create users
    fhqtest.print_bold("Try create " + user1_data['email'] + "...")
    user1 = fhqtest.admin_session.users_add(user1_data)
    fhqtest.check_response(user1, "User1 succesfull created")

    fhqtest.print_bold("Try create user2...")
    user2 = fhqtest.admin_session.users_add(user2_data)
    fhqtest.check_response(user2, "User2 succesfull created")

    fhqtest.print_bold("Try create user3...")
    user3 = fhqtest.admin_session.users_add(user3_data)
    fhqtest.check_response(user3, "User3 succesfull created")

    fhqtest.print_bold("Check unique user3 by email...")
    user3_again = fhqtest.admin_session.users_add(user3_data_again)
    fhqtest.alert(user3_again == None, 'Could not get response (users_add/user3 again)')
    if user3_again['result'] == "DONE":
        fhqtest.log_err("Wrong creating user email must be unique")
    else:
        fhqtest.print_success("User3 again - OK")

    fhqtest.print_bold("Check unique user by uuid...")
    user3_again2 = fhqtest.admin_session.users_add({"uuid": USER3_UUID, "email": "user3_test", "nick": "nick_user3_again", "password": "user3", "role": "user"})
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
        fhqtest.throw_err("Not found field 'country' in response after user update")
    elif user2_update["country"] != "Some country":
        fhqtest.throw_err("Field 'country' expected value of 'Some country'")
    if "university" not in user2_update:
        fhqtest.throw_err("Not found field 'university' in response after user update")
    elif  user2_update["university"] != "Some university":
        fhqtest.throw_err("Field 'university' expected value of 'Some university'")
    if "nick" not in user2_update:
        fhqtest.throw_err("Not found field 'nick' in response after user update")
    if "about" not in user2_update:
        fhqtest.throw_err("Not found field 'about' in response after user update")
    if "created" not in user2_update:
        fhqtest.throw_err("Not found field 'created' in response after user update")
    if "region" not in user2_update:
        fhqtest.throw_err("Not found field 'region' in response after user update")
    if "email" not in user2_update:
        fhqtest.throw_err("Not found field 'email' in response after user update")
    if "role" not in user2_update:
        fhqtest.throw_err("Not found field 'role' in response after user update")
    # TODO
    # if "updated" not in user2_update:
    #     fhqtest.log_err("Not found field 'updated' in response after user update")
    #     exit(-1)
    if "uuid" not in user2_update:
        fhqtest.log_err("Not found field 'uuid' in response after user update")
        exit(-1)

    test_login('user1')
    test_login('user2')
    test_login('user3')

    fhqtest.print_bold("Login by user2... ")
    user2_session = FreeHackQuestClient(fhqtest.TEST_SERVER)
    user2_login = user2_session.login({"email": "user2", "password": "user2"})
    fhqtest.alert(user2_login == None, 'Could not login as user2 (1)')
    fhqtest.alert(user2_login['result'] == 'FAIL', 'Could not login as user2 (2) (fail)')
    if user2_login['result'] == 'DONE':
        fhqtest.print_success("User2 login success - OK")
        fhqtest.print_bold("User tokens... ")
        user2_tokens = user2_session.users_tokens({})
        user2_session.close()
        user2_session = None
        if user2_tokens['result'] != 'DONE':
            fhqtest.log_err("User2 tokens failed")
            exit(-1)
        else:
            fhqtest.print_success("User2 tokens success - OK")

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
        fhqtest.throw_err("Not found field 'email' in response user2_found1")
    elif user2_found1['email'] != 'user2':
        fhqtest.throw_err("Expected field 'email' value 'user2', but got '" + user2_found1['email'] + "'")
        exit(-1)

    '''curr_user = fhqtest.admin_session.users_info({})
    fhqtest.alert(curr_user == None, 'Could not get user2')
    fhqtest.check_response(curr_user, "User2 succesfull got (2)")
    if "email" not in curr_user:
        fhqtest.log_err("Not found field 'email' in response curr_user")
        exit(-1)
    elif curr_user['email'] != 'user2':
        fhqtest.log_err("Expected field 'email' value 'user2', but got '" + curr_user['email'] + "'")
        exit(-1)'''


    user2_skills = fhqtest.admin_session.user_skills({"userid": user_found["id"]})
    fhqtest.alert(user2_skills == None, 'Could not get user skills')
    fhqtest.check_response(user2_skills, "User2 succesfull got skills")
    pprint(user2_skills)

    user3_remove = fhqtest.admin_session.user_delete({"userid": user3['data']['userid'], "password": fhqtest.ADMIN_PASSWORD})
    fhqtest.alert(user3_remove == None, 'Could not remove user')
    fhqtest.check_response(user3_remove, "User3 succesfull removed")

    user2_remove = fhqtest.admin_session.user_delete({"userid": user_found["id"], "password": fhqtest.ADMIN_PASSWORD})
    fhqtest.alert(user2_remove == None, 'Could not remove user')
    fhqtest.check_response(user2_remove, "User2 succesfull removed")



    # test user_change_password

    # test user login by different password

    # will not test user_reset_password
    # test login
    # test logout
    # test user_skills
    # test scoreboard
