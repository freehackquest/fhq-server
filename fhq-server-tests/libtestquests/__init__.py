#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from libfreehackquestclient import FreeHackQuestClient
import fhqtest
import sys, traceback
import base64
from pprint import pprint

test_name = 'Testing Quests'

def run_tests():
    # Cleanup
    fhqtest.clean_all_quests()

    gameid = fhqtest.find_first_gameid()

    # TODO symlinks to quests_create
    quest_name = fhqtest.generate_random(255).strip()
    quest_text = fhqtest.generate_random(300).strip()
    quest_answer = fhqtest.generate_random(255).strip()
    quest_answer_format = fhqtest.generate_random(255).strip()
    quest_author = fhqtest.generate_random(50).strip()
    quest_state = "open"
    quest_subject = "trivia"
    quest_score = 555
    quest_description_state = fhqtest.generate_random(255).strip()
    quest_copyright = fhqtest.generate_random(100).strip()

    fhqtest.print_bold("Try create quest...")
    quest1 = fhqtest.admin_session.createquest({
        "uuid": fhqtest.QUEST_UUID1,
        "gameid": gameid,
        "name": quest_name,
        "text": quest_text,
        "score": quest_score,
        "author": quest_author,
        "subject": quest_subject,
        "answer": quest_answer,
        "answer_format": quest_answer_format,
        "state": quest_state,
        "description_state": quest_description_state,
        "copyright": quest_copyright,
    })
    fhqtest.alert(quest1 == None, 'Could not get response (2)')
    fhqtest.check_response(quest1, "Quest succesfull created")
    questid1 = quest1['questid']

    quest1_new = fhqtest.admin_session.quest({"questid": questid1})
    quest1_new_hints = quest1_new['hints']
    quest1_new_files = quest1_new['files']
    quest1_new_game = quest1_new['game']
    quest1_new = quest1_new['quest']
    pprint(quest1_new)

    fhqtest.check_values("name of quest", quest1_new['name'], quest_name)
    fhqtest.check_values("author of quest", quest1_new['author'], quest_author)
    fhqtest.check_values("answer of quest", quest1_new['answer'], quest_answer)
    fhqtest.check_values("answer format of quest", quest1_new['answer_format'], quest_answer_format)
    fhqtest.check_values("description_state of quest", quest1_new['description_state'], quest_description_state)
    fhqtest.check_values("score of quest", quest1_new['score'], quest_score)
    fhqtest.check_values("state of quest", quest1_new['state'], quest_state)
    fhqtest.check_values("subject of quest", quest1_new['subject'], quest_subject)
    fhqtest.check_values("text of quest", quest1_new['text'], quest_text)
    fhqtest.check_values("copyright of quest", quest1_new['copyright'], quest_copyright)

    quest1_hint1 = fhqtest.generate_random(255).strip()
    quest1_hint2 = fhqtest.generate_random(255).strip()
    quest1_hint3 = fhqtest.generate_random(255).strip()

    fhqtest.print_bold("Add hint to quest...")
    h1 = fhqtest.admin_session.addhint({"questid": questid1, "hint": quest1_hint1})
    fhqtest.alert(h1 == None, 'Could not get response (hint1)')
    fhqtest.check_response(h1, "Quest hint succesfull created")
    pprint(h1)

    h2 = fhqtest.admin_session.addhint({"questid": questid1, "hint": quest1_hint2})
    fhqtest.alert(h2 == None, 'Could not get response (hint2)')
    fhqtest.check_response(h2, "Quest hint succesfull created")
    pprint(h2)

    h3 = fhqtest.admin_session.addhint({"questid": questid1, "hint": quest1_hint3})
    fhqtest.alert(h3 == None, 'Could not get response (hint3)')
    fhqtest.check_response(h3, "Quest hint succesfull created")
    pprint(h3) # TODO return id and text

    fhqtest.print_bold("Try update quest...")
    quest_name2 = fhqtest.generate_random(255).strip()
    quest_text2 = fhqtest.generate_random(300).strip()
    quest_answer2 = fhqtest.generate_random(255).strip()
    quest_answer_format2 = fhqtest.generate_random(255).strip()
    quest_author2 = fhqtest.generate_random(50).strip()
    quest_state2 = "broken"
    quest_subject2 = "stego"
    quest_score2 = 456
    quest_description_state2 = fhqtest.generate_random(255).strip()
    quest_copyright2 = fhqtest.generate_random(100).strip()

    quest1_update = fhqtest.admin_session.quest_update({
        "questid": questid1,
        "name": quest_name2,
        "gameid": gameid,
        "text": quest_text2,
        "score": quest_score2,
        "author": quest_author2,
        "subject": quest_subject2,
        "answer": quest_answer2,
        "answer_format": quest_answer_format2,
        "state": quest_state2,
        "description_state": quest_description_state2,
        "copyright": quest_copyright2,
    })
    fhqtest.alert(quest1_update == None, 'Could not get response (4)')
    fhqtest.check_response(quest1_update, "Quest succesfull update")
    
    quest1_updated = fhqtest.admin_session.quest({"questid": questid1})
    quest1_updated_hints = quest1_updated['hints']
    quest1_updated_files = quest1_updated['files']
    quest1_updated_game = quest1_updated['game']
    quest1_updated = quest1_updated['quest']
    print("quest1_updated = " + str(quest1_updated))

    fhqtest.check_values("name of quest", quest1_updated['name'], quest_name2)
    fhqtest.check_values("author of quest", quest1_updated['author'], quest_author2)
    fhqtest.check_values("answer of quest", quest1_updated['answer'], quest_answer2)
    fhqtest.check_values("answer format of quest", quest1_updated['answer_format'], quest_answer_format2)
    fhqtest.check_values("description_state of quest", quest1_updated['description_state'], quest_description_state2)
    fhqtest.check_values("score of quest", quest1_updated['score'], quest_score2)
    fhqtest.check_values("state of quest", quest1_updated['state'], quest_state2)
    fhqtest.check_values("subject of quest", quest1_updated['subject'], quest_subject2)
    fhqtest.check_values("text of quest", quest1_updated['text'], quest_text2)
    fhqtest.check_values("copyright of quest", quest1_updated['copyright'], quest_copyright2)

    fhqtest.check_values("hint1 of quest", quest1_updated_hints[0]['text'], quest1_hint1)
    fhqtest.check_values("hint2 of quest", quest1_updated_hints[1]['text'], quest1_hint2)
    fhqtest.check_values("hint3 of quest", quest1_updated_hints[2]['text'], quest1_hint3)

    # try remove hint
    h2_remove = fhqtest.admin_session.deletehint({"hintid": quest1_updated_hints[1]['id']})
    fhqtest.alert(h2_remove == None, 'Could not get response (hint2_remove)')
    fhqtest.check_response(h2_remove, "Quest hint2 succesfull removed")

    fhqtest.print_bold("Try upload file to quest...")
    # pprint(quest1_updated)
    quest_uuid = quest1_updated['uuid']
    resp = fhqtest.admin_session.quests_files_upload({
        "quest_uuid": quest_uuid,
        "file_base64": "SGVsbG8gd29ybGQh",
        "file_name": "test1.txt",
    })
    fhqtest.alert(resp['result'] == 'FAIL', 'Wrong response')
    fhqtest.check_response(resp, "Quest quests_files_upload succesfull")

    questid = quest1_updated['id']
    resp = fhqtest.admin_session.quests({"gameid": gameid})
    fhqtest.alert(resp == None, 'Could not get response (quests)')
    fhqtest.check_response(resp, "quest get filtered")
    fhqtest.alert(resp['result'] == 'FAIL', 'Wrong response')
    fhqtest.alert(resp['data'][0]['questid'] != questid, 'Wrong work filter by gameid in request quests')

    # 
    fhqtest.print_bold("Try answerlist...")
    r_answerlist = fhqtest.admin_session.answerlist({"page": 0, "onpage": 10})
    fhqtest.alert(r_answerlist == None, 'Could not get response (r_answerlist)')
    fhqtest.check_response(r_answerlist, "Answer list succesfull got")
    pprint(r_answerlist)
    # TODO check this


    # Cleanup
    fhqtest.clean_all_quests()



