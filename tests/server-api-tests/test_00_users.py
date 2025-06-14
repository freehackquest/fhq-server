#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2011-2025, Evgenii Sopov <mrseakg@gmail.com>

"""
Test Server Settings
"""

from freehackquest_libclient_py import FreeHackQuestClient

def test_0001_cleanup_users(admin_session, admin_password):
    """ test_0001_cleanup_users """
    print(test_0001_cleanup_users.__doc__)
    _user_email = [
        "user1",
        "user2",
        "user3",
        "user3_test",
    ]

    for _user in _user_email:
        users = admin_session.users({
            "onpage": 10,
            "page": 0,
            "filter_text": _user
        })
        user = None
        for usr in users["data"]:
            if usr['email'] == _user:
                user = usr
            if usr['created'] == '':
                assert "Field created could not be empty for user #" + str(usr['id']) == ''
        if user is not None:
            print("Removing '" + _user + "' ...")
            user_delete = admin_session.users_delete({
                "userid": user['id'],
                "password": admin_password
            })
            assert user_delete is not None
            assert user_delete["result"] == "DONE"
            print("User '" + _user + "' succesfull removed")

def test_0002_create_users(admin_session, user1_data, user2_data, user3_data, user3_data_again):
    """ test_0002_create_users """
    print(test_0002_create_users.__doc__)
    user1 = admin_session.users_add(user1_data)
    assert user1 is not None
    assert user1["result"] == "DONE"
    print("User1 succesfull created")

    user2 = admin_session.users_add(user2_data)
    assert user2 is not None
    assert user2["result"] == "DONE"
    print("User2 succesfull created")

    user3 = admin_session.users_add(user3_data)
    assert user3 is not None
    assert user3["result"] == "DONE"
    print("User3 succesfull created")

    user3_again = admin_session.users_add(user3_data_again)
    assert user3_again is not None
    assert user3_again["result"] == "FAIL"

def test_0003_find_user2_and_update(admin_session, user2_data, test_server):
    """ test_0003_find_user2_and_update """
    print(test_0003_find_user2_and_update.__doc__)

    users = admin_session.users({
        "onpage": 10,
        "page": 0,
        "filter_text": user2_data["nick"]
    })
    user2_found = None
    for _user in users["data"]:
        if _user['email'] == user2_data["email"]:
            user2_found = _user
    assert user2_found is not None
    assert user2_found["country"] == ''

    user2_update = admin_session.user_update({
        "userid": user2_found["id"],
        "country": "Some country",
        "university": "Some university",
    })
    assert user2_update is not None
    assert user2_update["result"] == "DONE"
    user2_update = user2_update["data"]
    assert "country" in user2_update
    assert user2_update["country"] == "Some country"
    assert "university" in user2_update
    assert user2_update["university"] == "Some university"
    assert "nick" in user2_update
    assert user2_update["nick"] == user2_data["nick"]
    assert "about" in user2_update
    assert "created" in user2_update
    assert "region" in user2_update
    assert "email" in user2_update
    assert user2_update["email"] == user2_data["email"]
    assert "role" in user2_update
    assert "uuid" in user2_update
    # assert user2_update["uuid"] == user2_data["uuid"] ???
    # assert "updated" in user2_update

    # test login by user2
    user2_session = FreeHackQuestClient(test_server)
    user2_login = user2_session.login({
        "email": user2_data["email"],
        "password": user2_data["password"]
    })
    assert user2_login is not None
    assert user2_login["result"] == "DONE"

    # request list of tokens
    user2_tokens = user2_session.users_tokens({})
    assert user2_tokens is not None
    assert user2_login["result"] == "DONE"
    user2_session.close()

    user2_found2 = admin_session.user({"userid": user2_found["id"]})
    assert user2_found2 is not None
    assert user2_found2["result"] == "DONE"
    user2_found2 = user2_found2["data"]
    assert "email" in user2_found2
    assert user2_found2["email"] == user2_data["email"]

    # curr_user = admin_session.users_info({})
    # assert curr_user is not None
    # assert curr_user["result"] == "DONE"
    # curr_user = curr_user['data']
    # assert "email" in curr_user
    # assert curr_user["email"] == 'admin'

    user2_skills = admin_session.user_skills({"userid": user2_found["id"]})
    assert user2_skills is not None
    assert user2_skills["result"] == "DONE"

def test_0004_remove_user3(admin_session, user3_data, admin_password):
    """ test_0004_remove_user3 """
    print(test_0004_remove_user3.__doc__)
    users = admin_session.users({
        "onpage": 10,
        "page": 0,
        "filter_text": user3_data["nick"]
    })
    assert users is not None
    user3_found = None
    for _user in users["data"]:
        if _user['email'] == user3_data["email"]:
            user3_found = _user
    assert user3_found is not None
    print(user3_found)

    user3_remove = admin_session.users_delete({
        "userid": user3_found['id'],
        "password": admin_password
    })
    assert user3_remove is not None
    assert user3_remove["result"] == "DONE"
