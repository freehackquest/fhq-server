#!/usr/bin/env python3
# Copyright (C) 2011-2025, Evgenii Sopov <mrseakg@gmail.com>


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
