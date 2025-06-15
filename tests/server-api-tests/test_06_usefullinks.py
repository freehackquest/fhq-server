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

"""
Test server api leaks
"""


def test_0000_cleanup_useful_links(admin_session):
    """ Cleanup useful_links list """
    print(test_0000_cleanup_useful_links.__doc__)
    print(admin_session)
    ul_list = admin_session.useful_links_list({
        "filter": ""
    })
    assert ul_list is not None
    assert ul_list["result"] is not None
    assert ul_list["result"] == 'DONE'

    for _ul in ul_list['data']['items']:
        ul_uuid = _ul['uuid']
        print("removing " + str(ul_uuid))
        admin_session.useful_links.remove({"useful_link_uuid": ul_uuid})


def test_0001_add_useful_links(admin_session, generate_random):
    """ Add """
    print(test_0001_add_useful_links.__doc__)
    print(admin_session)

    url1 = generate_random(2048)
    description1 = generate_random(2048)
    author1 = generate_random(127)

    ul_item1 = admin_session.useful_links_add({
        "url": url1,
        "description": description1,
        "author": author1,
    })
    assert ul_item1 is not None
    assert ul_item1["result"] == "DONE"

# useful_links_clicked
# useful_links_comment_add
# useful_links_comment_delete
# useful_links_comment_list
# useful_links_delete
# useful_links_retrieve
# useful_links_tag_add
# useful_links_tag_delete
# useful_links_tag_list
# useful_links_update
# useful_links_user_favorite
# useful_links_user_favorite_list
# useful_links_user_propose
# useful_links_user_propose_approve
# useful_links_user_unfavorite
