#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2011-2023, Evgenii Sopov <mrseakg@gmail.com>


""" classbook """


def test_0001_quests(admin_session, generate_random, generate_random_uuid):
    """ test_0002_add_items """
    print(test_0001_quests.__doc__)

    games1 = admin_session.games_list({})
    assert games1['result'] == 'DONE'
    games1 = games1['data']
    if len(games1) == 0:
        print("ERROR: Not found any game")
        assert len(games1) == 0
    gameid = games1[0]['local_id']

    # to quests_create
    quest_data =  {
        "uuid": generate_random_uuid(),
        "gameid": gameid,
        "name": generate_random(255).strip(),
        "text": generate_random(300).strip(),
        "score": 555,
        "author": generate_random(50).strip(),
        "subject": "trivia",
        "answer": generate_random(255).strip(),
        "answer_format": generate_random(255).strip(),
        "state": "open",
        "description_state": generate_random(255).strip(),
        "copyright": generate_random(255).strip(),
    }

    quest1 = admin_session.quests.add(quest_data)
    assert quest1['result'] == 'DONE'
    questid1 = quest1['questid']

    quest1_ret = admin_session.quest({"questid": questid1})
    # quest1_new_hints = quest1_new['hints']
    # quest1_new_files = quest1_new['files']
    # quest1_new_game = quest1_new['game']
    quest1_ret = quest1_ret['quest']
    # pprint(quest1_ret)

    assert quest1_ret['name'] == quest_data['name']
    assert quest1_ret['author'] == quest_data['author']
    assert quest1_ret['answer'] == quest_data['answer']
    assert quest1_ret['answer_format'] == quest_data['answer_format']
    assert quest1_ret['description_state'] == quest_data['description_state']
    assert quest1_ret['score'] == quest_data['score']
    assert quest1_ret['state'] == quest_data['state']
    assert quest1_ret['subject'] == quest_data['subject']
    assert quest1_ret['text'] == quest_data['text']
    assert quest1_ret['copyright'] == quest_data['copyright']

    # quest1_hint1 = generate_random(255).strip()
    # quest1_hint2 = generate_random(255).strip()
    # quest1_hint3 = generate_random(255).strip()

    # fhqtest.print_bold("Add hint to quest...")
    # resp_h1 = fhqtest.admin_session.addhint({"questid": questid1, "hint": quest1_hint1})
    # fhqtest.alert(resp_h1 is None, 'Could not get response (hint1)')
    # fhqtest.check_response(resp_h1, "Quest hint succesfull created")
    # pprint(resp_h1)

    # h2 = fhqtest.admin_session.addhint({"questid": questid1, "hint": quest1_hint2})
    # fhqtest.alert(h2 is None, 'Could not get response (hint2)')
    # fhqtest.check_response(h2, "Quest hint succesfull created")
    # pprint(h2)

    # h3 = fhqtest.admin_session.addhint({"questid": questid1, "hint": quest1_hint3})
    # fhqtest.alert(h3 is None, 'Could not get response (hint3)')
    # fhqtest.check_response(h3, "Quest hint succesfull created")
    # pprint(h3) # return id and text

    # fhqtest.print_bold("Try update quest...")
    # quest_name2 = fhqtest.generate_random(255).strip()
    # quest_text2 = fhqtest.generate_random(300).strip()
    # quest_answer2 = fhqtest.generate_random(255).strip()
    # quest_answer_format2 = fhqtest.generate_random(255).strip()
    # quest_author2 = fhqtest.generate_random(50).strip()
    # quest_state2 = "broken"
    # quest_subject2 = "stego"
    # quest_score2 = 456
    # quest_description_state2 = fhqtest.generate_random(255).strip()
    # quest_copyright2 = fhqtest.generate_random(100).strip()

    # quest1_update = fhqtest.admin_session.quest_update({
    #     "questid": questid1,
    #     "name": quest_name2,
    #     "gameid": gameid,
    #     "text": quest_text2,
    #     "score": quest_score2,
    #     "author": quest_author2,
    #     "subject": quest_subject2,
    #     "answer": quest_answer2,
    #     "answer_format": quest_answer_format2,
    #     "state": quest_state2,
    #     "description_state": quest_description_state2,
    #     "copyright": quest_copyright2,
    # })
    # fhqtest.alert(quest1_update is None, 'Could not get response (4)')
    # fhqtest.check_response(quest1_update, "Quest succesfull update")

    # quest1_updated = fhqtest.admin_session.quest({"questid": questid1})
    # quest1_updated_hints = quest1_updated['hints']
    # # quest1_updated_files = quest1_updated['files']
    # # quest1_updated_game = quest1_updated['game']
    # quest1_updated = quest1_updated['quest']
    # print("quest1_updated = " + str(quest1_updated))

    # fhqtest.check_values("name of quest", quest1_updated['name'], quest_name2)
    # fhqtest.check_values("author of quest", quest1_updated['author'], quest_author2)
    # fhqtest.check_values("answer of quest", quest1_updated['answer'], quest_answer2)
    # fhqtest.check_values(
    #     "answer format of quest", quest1_updated['answer_format'], quest_answer_format2
    # )
    # fhqtest.check_values(
    #     "description_state of quest", quest1_updated['description_state'],
    #   quest_description_state2
    # )
    # fhqtest.check_values("score of quest", quest1_updated['score'], quest_score2)
    # fhqtest.check_values("state of quest", quest1_updated['state'], quest_state2)
    # fhqtest.check_values("subject of quest", quest1_updated['subject'], quest_subject2)
    # fhqtest.check_values("text of quest", quest1_updated['text'], quest_text2)
    # fhqtest.check_values("copyright of quest", quest1_updated['copyright'], quest_copyright2)

    # fhqtest.check_values("hint1 of quest", quest1_updated_hints[0]['text'], quest1_hint1)
    # fhqtest.check_values("hint2 of quest", quest1_updated_hints[1]['text'], quest1_hint2)
    # fhqtest.check_values("hint3 of quest", quest1_updated_hints[2]['text'], quest1_hint3)

    # # try remove hint
    # h2_remove = fhqtest.admin_session.deletehint({"hintid": quest1_updated_hints[1]['id']})
    # fhqtest.alert(h2_remove is None, 'Could not get response (hint2_remove)')
    # fhqtest.check_response(h2_remove, "Quest hint2 succesfull removed")

    # fhqtest.print_bold("Try upload file to quest...")
    # # pprint(quest1_updated)
    # quest_uuid = quest1_updated['uuid']
    # resp = fhqtest.admin_session.quests_files.upload({
    #     "quest_uuid": quest_uuid,
    #     "file_base64": "SGVsbG8gd29ybGQh",
    #     "file_name": "test1.txt",
    # })
    # fhqtest.alert(resp['result'] == 'FAIL', 'Wrong response')
    # fhqtest.check_response(resp, "Quest quests_files_upload succesfull")

    # questid = quest1_updated['id']
    # resp = fhqtest.admin_session.quests({"gameid": gameid})
    # fhqtest.alert(resp is None, 'Could not get response (quests)')
    # fhqtest.check_response(resp, "quest get filtered")
    # fhqtest.alert(resp['result'] == 'FAIL', 'Wrong response')
    # fhqtest.alert(
    #     resp['data'][0]['questid'] != questid, 'Wrong work filter by gameid in request quests'
    # )

    # fhqtest.user_session.quest_pass({
    #     "questid": quest1_updated['id'],
    #     "answer": "some answer 1"
    # })
    # time.sleep(1.0)
    # fhqtest.user_session.quest_pass({
    #     "questid": quest1_updated['id'],
    #     "answer": "some answer 2"
    # })
    # time.sleep(1.0)
    # resp = fhqtest.user_session.quest_pass({
    #     "questid": quest1_updated['id'],
    #     "answer": quest1_updated['answer']
    # })
    # fhqtest.check_response(resp, "Quest quest_pass succesfull")

    # resp = fhqtest.user_session.quest_statistics({"questid": quest1_updated['id']})
    # fhqtest.alert(resp['result'] == 'FAIL', 'Wrong response')
    # fhqtest.check_values("quest solved", resp['solved'], 1)
    # fhqtest.check_values("quest tries", resp['tries'], 2)
    # resp = fhqtest.user_session.quests_user_answer_list({"questid": quest1_updated['id']})
    # pprint(resp)
    # fhqtest.print_bold("Try answerlist...")
    # resp = fhqtest.admin_session.answerlist({"page": 0, "onpage": 10})
    # fhqtest.alert(resp['result'] == 'FAIL', 'Could not get response (r_answerlist)')
    # fhqtest.check_response(resp, "Answer list succesfull got")
    # fhqtest.check_values("last quest passed 0", resp['data'][0]['passed'], 'Yes')
    # fhqtest.check_values("last quest passed 1", resp['data'][1]['passed'], 'No')
    # fhqtest.check_values(
    #     "last quest passed 1 answer", resp['data'][1]['user_answer'], 'some answer 2'
    # )
    # fhqtest.check_values("last quest passed 2", resp['data'][2]['passed'], 'No')
    # fhqtest.check_values(
    #     "last quest passed 1 answer", resp['data'][2]['user_answer'], 'some answer 1'
    # )
    # # Warning: Command quest_proposal 0 times
    # # Cleanup
    # fhqtest.clean_all_quests()
