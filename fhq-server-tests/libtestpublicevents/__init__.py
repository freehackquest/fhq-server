#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from libfreehackquestclient import FreeHackQuestClient
import fhqtest
import sys, traceback
import base64
import time
from pprint import pprint

test_name = 'Testing Public Events'

def run_tests():
   
    r = fhqtest.admin_session.publiceventslist({"page": 0, "onpage": 10})
    if r['count'] > 0:
        pass
        # TODO remove
    
    event_message = fhqtest.generate_random(50).strip()
    event_message_correct = event_message
    event_message_correct = event_message_correct.replace('>', '&gt;')
    event_message_correct = event_message_correct.replace('<', '&lt;')
    event_message_correct = event_message_correct.replace('"', '&quot;')

    event_type = "info"
    print(event_message)

    r_pub_event1 = fhqtest.admin_session.createpublicevent({
        "type": event_type,
        "message": event_message
    })
    fhqtest.alert(r_pub_event1 == None, 'Could not get response (createpublicevent)')
    fhqtest.check_response(r_pub_event1, "Public Event succesfull created")
    pprint(r_pub_event1)
    time.sleep(1)

    # TODO redesign on previus request must return data
    r = fhqtest.admin_session.publiceventslist({"page": 0, "onpage": 10, "search": event_message})
    pub_event1 = r['data'][0]
    
    fhqtest.check_values("type of public event", pub_event1['type'], event_type)
    fhqtest.check_values("message of public event", pub_event1['message'], event_message_correct)

    r_event1 = fhqtest.admin_session.getpublicevent({"eventid": pub_event1['id']})
    fhqtest.alert(r_event1 == None, 'Could not get response (getpublicevent)')
    fhqtest.check_response(r_event1, "Public Event succesfull got")
    r_event1 = r_event1['data']
    fhqtest.check_values("(2) type of public event", r_event1['type'], event_type)
    fhqtest.check_values("(2) message of public event", r_event1['message'], event_message_correct)

    r_event1_removed = fhqtest.admin_session.deletepublicevent({"eventid": pub_event1['id']})
    fhqtest.alert(r_event1_removed == None, 'Could not get response (deletepublicevent)')
    fhqtest.check_response(r_event1_removed, "Public Event succesfull removed")

