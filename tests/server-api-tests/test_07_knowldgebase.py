#!/usr/bin/env python3
##################################################################################
#                      Project
#    __ _
#   / _| |__   __ _       ___  ___ _ ____   _____ _ __
#  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
#  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
#  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
#                |_|
#
# MIT License
#
# Copyright (c) 2011-2026 FreeHackQuest <freehackquest@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Original repository: https://github.com/freehackquest/fhq-server
#
##################################################################################


""" classbook """

from pprint import pprint


def remove_item(admin_session, classbookid, padding):
    """ remove item """
    records_list = admin_session.classbook_list({
        "parentid": classbookid,
    })
    childs = len(records_list['data'])
    print("     * " + padding + " " + str(classbookid) + " has " + str(childs) + " childs")
    # check_response(records_list, "Classbook Records list got")
    for clb in records_list['data']:
        childs = childs + 1
        child_classbookid = clb['classbookid']
        remove_item(
            admin_session,
            child_classbookid,
            padding + "-(" + str(child_classbookid) + " has " + str(clb['childs']) + " childs)-"
        )
    if classbookid != 0:
        # print("Try remove classbook record #" + str(child_classbookid))
        resp = admin_session.classbook_delete_record({"classbookid": classbookid})
        # check_response(r, "Record #" + str(child_classbookid) + " succesfull removed")
        if resp['result'] == 'FAIL':
            pprint(resp)
            raise ValueError('Could not remove classbook record #' + str(classbookid))
        print(" * " + padding + "+ " + str(classbookid) + " - removed")


# def test_0001_clean_classbook(admin_session):
#     """ test_0001_clean_classbook """
#     print(test_0001_clean_classbook.__doc__)
#     remove_item(admin_session, 0, "-")


def test_0002_add_items(admin_session, generate_random, generate_random_uuid):
    """ test_0002_add_items """
    print(test_0002_add_items.__doc__)
    records = []
    for _ in range(10):
        records.append({
            'name': generate_random(10),
            'content': generate_random(255),
            'uuid': generate_random_uuid(),
        })

    next_parentid = 0
    for rec in records:
        rec['parentid'] = next_parentid
        resp = admin_session.classbook_add_record({
            "parentid": rec['parentid'],
            "name": rec['name'],
            "content": rec['content'],
            "uuid": rec['uuid'],
            "ordered": 0
        })
        assert resp["result"] == "DONE"
        print(rec)
        print(
            "Record " + rec['uuid'] + " succesfull in parent " +
            str(rec['parentid']) + " created"
        )
        rec['classbookid'] = resp['data']['classbookid']
        next_parentid = rec['classbookid']

    records = sorted(records, key=lambda x: x['classbookid'], reverse=True)
    pprint(records)

    # delete
    for rec in records:
        print(rec)
        resp = admin_session.classbook_delete_record({"classbookid": rec['classbookid']})
        if resp["result"] == "FAIL":
            print(resp)
        assert resp["result"] == "DONE"

# classbook_export 0 times
# classbook_info 0 times
# classbook_localization_add_record 0 times
# classbook_localization_delete_record 0 times
# classbook_localization_info 0 times
# classbook_localization_update_record 0 times
# classbook_propasal_approve 0 times
# classbook_propasal_prepare_merge_record 0 times
# classbook_propasal_update 0 times
# classbook_proposal_add_record 0 times
# classbook_proposal_delete_record 0 times
# classbook_proposal_info 0 times
# classbook_proposal_list 0 times
# classbook_update_record 0 times
