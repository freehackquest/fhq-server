#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Test Scoreboard
"""

def test_scoreboard_get(admin_session):
    """ Scoreboard Get """
    print(test_scoreboard_get.__doc__)
    scoreboard = admin_session.scoreboard({"page": 0, "onpage": 10})
    assert scoreboard is not None
    assert scoreboard["result"] == "DONE"
  