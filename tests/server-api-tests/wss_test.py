#!/usr/bin/env python3
# Copyright (C) 2011-2025, Evgenii Sopov <mrseakg@gmail.com>

"""
    Script for a testing connection to production server
"""

from freehackquest_libclient_py import FreeHackQuestClient

print("WS:")

ADMIN_EMAIL = "admin"
ADMIN_PASSWORD = "admin"

ws_admin = FreeHackQuestClient("ws://freehackquest.com/api-ws/")
resp = ws_admin.login({
    "email": ADMIN_EMAIL,
    "password": ADMIN_PASSWORD
})
print(resp)

print("WSS:")
wss_admin = FreeHackQuestClient("wss://freehackquest.com/api-wss/")
resp = wss_admin.login({
    "email": ADMIN_EMAIL,
    "password": ADMIN_PASSWORD
})
print(resp)
