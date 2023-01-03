#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2011-2023, Evgenii Sopov <mrseakg@gmail.com>

"""
Test server api leaks
"""

def test_0000_cleanup_useful_links(admin_session):
    """Cleanup useful_links list"""
    print(test_0000_cleanup_useful_links.__doc__)
    print(admin_session)
    ul_list = admin_session.useful_links_list({ "filter": "" })
    assert ul_list is not None
    assert ul_list["result"] == "DONE"

    for _ul in ul_list['data']['items']:
        ul_id = _ul['id']
        print("remove " + str(ul_id))
        admin_session.useful_links_delete({"useful_link_id": ul_id})

def test_0001_add_useful_links(admin_session, generate_random):
    """Add """
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
