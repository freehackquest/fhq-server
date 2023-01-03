#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2011-2023, Evgenii Sopov <mrseakg@gmail.com>

"""
Test web server
"""

import requests

def test_web_server(url_http_web_server):
    """ Coverage """
    print(test_web_server.__doc__)

    print("Request main admin page... ")

    _resp = requests.get(url_http_web_server + 'admin/')
    assert _resp.status_code == 200
