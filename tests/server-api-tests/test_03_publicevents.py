#!/usr/bin/env python3
# Copyright (C) 2011-2025, Evgenii Sopov <mrseakg@gmail.com>

"""
Test Public Events
"""

import random
import string
import time

def generate_random(size):
    """Generate random printable string"""
    _range = range(size)
    _alphabet = string.ascii_uppercase + string.digits + ' _+=\'"~@!#?/<>'
    return ''.join(random.choice(_alphabet) for _ in _range)

def test_publicevents_base(admin_session):
    """ Test Public events base """
    print(test_publicevents_base.__doc__)
    events = admin_session.publiceventslist({"page": 0, "onpage": 10})
    assert events is not None
    assert events["result"] == "DONE"
    if events["count"] > 0:
        pass # remove all current events
    event_message = generate_random(50).strip()
    event_message_correct = event_message
    event_message_correct = event_message_correct.replace('>', '&gt;')
    event_message_correct = event_message_correct.replace('<', '&lt;')
    event_message_correct = event_message_correct.replace('"', '&quot;')

    event_type = "info"
    print(event_message)

    r_pub_event1 = admin_session.createpublicevent({
        "type": event_type,
        "message": event_message
    })
    assert r_pub_event1 is not None
    assert r_pub_event1["result"] == "DONE"
    print(r_pub_event1)

    time.sleep(1)
    # redesign on previus request must return data
    events = admin_session.publiceventslist({
        "page": 0, "onpage": 10, "search": event_message
    })
    assert events is not None
    assert events["result"] == "DONE"
    pub_event1 = events['data'][0]
    assert pub_event1['type'] == event_type
    assert pub_event1['message'] == event_message_correct

    r_event1 = admin_session.getpublicevent({"eventid": pub_event1['id']})
    assert r_event1 is not None
    assert r_event1["result"] == "DONE"
    r_event1 = r_event1['data']
    assert r_event1['type'] == event_type
    assert r_event1['message'] == event_message_correct

    r_event1_removed = admin_session.deletepublicevent({"eventid": pub_event1['id']})
    assert r_event1_removed is not None
    assert r_event1_removed["result"] == "DONE"
