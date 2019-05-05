#!/usr/bin/env python
# -*- coding: utf-8 -*-

import libfhqcli
import fhqtest
import sys, traceback
import base64
from pprint import pprint

test_name = 'Testing Quests'

try:
    fhqtest.print_header(" > > > " + test_name + ": begin ")
    fhqtest.init_enviroment()

    # Cleanup
    fhqtest.print_bold("Clean up test data... ")
    quests1 = fhqtest.admin_session.quests({})
    fhqtest.alert(quests1 == None, 'Could not get response')
    quests1 = quests1['data']
    for q in quests1:
        questid = q['questid']
        quest_removed = fhqtest.admin_session.quest_delete({"questid": questid})
        fhqtest.check_response(quest_removed, "Quest #" + str(questid) + " succesfull removed")
    
    fhqtest.print_bold("Search game id...")
    games1 = fhqtest.admin_session.games({})
    fhqtest.check_response(games1, "Games succesfull got")
    games1 = games1['data']
    gameid = 0
    if len(games1) == 0:
        fhqtest.log_err("Not found any game")
    gameid = games1[0]['local_id']

    # TODO symlinks to quests_create
    quest_name = fhqtest.generate_random(255)
    quest_text = fhqtest.generate_random(300)
    quest_answer = fhqtest.generate_random(255)
    quest_answer_format = fhqtest.generate_random(255)
    quest_author = fhqtest.generate_random(50)
    quest_state = fhqtest.generate_random(10)
    quest_subject = fhqtest.generate_random(10)
    quest_score = 555
    quest_description_state = fhqtest.generate_random(255)
    quest_copyright = fhqtest.generate_random(100)

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

except Exception as e:
    fhqtest.log_err(str(e))
    traceback.print_exc(file=sys.stdout)
finally:
    fhqtest.print_header(" < < < " + test_name + ": end ")
    fhqtest.deinit_enviroment()
