#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Test server api leaks
"""

import os
import random
import string
import base64
import shutil
import zipfile
import json
import requests

def generate_random(size):
    """Generate random printable string"""
    _range = range(size)
    _alphabet = string.ascii_uppercase + string.digits + ' _+=\'"~@!#?/<>'
    return ''.join(random.choice(_alphabet) for _ in _range)

def generate_random_uuid():
    """Generate random uuid"""
    ret = ''
    ret = ret + ''.join(random.choice('0123456789abcdef') for _ in range(8))
    ret = ret + '-'
    ret = ret + ''.join(random.choice('0123456789abcdef') for _ in range(4))
    ret = ret + '-'
    ret = ret + ''.join(random.choice('0123456789abcdef') for _ in range(4))
    ret = ret + '-'
    ret = ret + ''.join(random.choice('0123456789abcdef') for _ in range(4))
    ret = ret + '-'
    ret = ret + ''.join(random.choice('0123456789abcdef') for _ in range(12))
    return ret

def test_games_cleanup_game2(admin_session, game2_uuid, admin_password):
    """Cleanup test game"""
    print(test_games_cleanup_game2.__doc__)
    game2 = admin_session.game_info({ "uuid": game2_uuid })
    assert game2 is not None
    if game2['result'] == 'DONE':
        game2_delete_r = admin_session.game_delete({
            "uuid": game2_uuid,
            "admin_password": admin_password
        })
        assert game2_delete_r is not None
        assert game2_delete_r['result'] == 'DONE'
        print("Cleaned")

def test_games_create_game2(admin_session, game2_uuid):
    """Create game2"""
    print(test_games_create_game2.__doc__)
    game_name = generate_random(255)
    game_description = generate_random(300)
    game_teams = generate_random(255)
    game_state = "original"
    game_form = "online"
    game_type = "jeopardy"
    game_date_start = "2000-01-01 00:00:00"
    game_date_stop = "2001-01-01 00:00:00"
    game_date_restart = "2011-01-01 00:00:00"

    game2 = admin_session.game_create({
        "uuid": game2_uuid,
        "name": game_name,
        "description": game_description,
        "state": game_state,
        "form": game_form,
        "type": game_type,
        "date_start": game_date_start,
        "date_stop": game_date_stop,
        "date_restart": game_date_restart,
        "organizators": game_teams
    })
    assert game2 is not None
    assert game2['result'] == 'DONE'
    print(game2)
    assert game2['data']['uuid'] == game2_uuid
    assert game2['data']['name'] == game_name
    assert game2['data']['date_restart'] == game_date_restart
    assert game2['data']['date_start'] == game_date_start
    assert game2['data']['date_stop'] == game_date_stop
    assert game2['data']['description'] == game_description
    assert game2['data']['form'] == game_form
    assert game2['data']['maxscore'] == 0
    assert game2['data']['name'] == game_name
    assert game2['data']['organizators'] == game_teams
    assert game2['data']['state'] == game_state
    assert game2['data']['type'] == game_type

    game2 = admin_session.game_info({ "uuid": game2_uuid })
    assert game2 is not None
    assert game2['result'] == 'DONE'
    assert game2['data']['uuid'] == game2_uuid
    assert game2['data']['name'] == game_name
    assert game2['data']['date_restart'] == game_date_restart
    assert game2['data']['date_start'] == game_date_start
    assert game2['data']['date_stop'] == game_date_stop
    assert game2['data']['description'] == game_description
    assert game2['data']['form'] == game_form
    assert game2['data']['maxscore'] == 0
    assert game2['data']['name'] == game_name
    assert game2['data']['organizators'] == game_teams
    assert game2['data']['state'] == game_state
    assert game2['data']['type'] == game_type
    gameid = game2['data']['local_id']
    print("gameid: " + str(gameid))

def test_games_game_list(admin_session, game2_uuid):
    """games list"""
    print(test_games_game_list.__doc__)
    games_list = admin_session.games_list({})
    assert games_list is not None
    _found = False
    for game_ in games_list['data']:
        if game_['uuid'] == game2_uuid:
            _found = True
    assert _found is True

def test_games_update_game_name(admin_session, game2_uuid):
    """game update name"""
    print(test_games_update_game_name.__doc__)

    game2_prev = admin_session.game_info({ "uuid": game2_uuid })
    assert game2_prev is not None
    game2_name = generate_random(255)
    game2_updt = admin_session.game_update({
        "uuid": game2_uuid,
        "name": game2_name,
    })
    assert game2_updt is not None
    game2_new = admin_session.game_info({ "uuid": game2_uuid })
    assert game2_new['data']['name'] == game2_name
    assert game2_prev['data']['organizators'] == game2_new['data']['organizators']
    assert game2_prev['data']['description'] == game2_new['data']['description']
    assert game2_prev['data']['maxscore'] == game2_new['data']['maxscore']

def test_games_update_game_description(admin_session, game2_uuid):
    """game update description"""
    print(test_games_update_game_description.__doc__)
    game2_prev = admin_session.game_info({ "uuid": game2_uuid })
    assert game2_prev is not None
    game2_description = generate_random(255)
    game2_updt = admin_session.game_update({
        "uuid": game2_uuid,
        "description": game2_description,
    })
    assert game2_updt is not None
    game2_new = admin_session.game_info({ "uuid": game2_uuid })
    assert game2_new['data']['description'] == game2_description
    assert game2_prev['data']['organizators'] == game2_new['data']['organizators']
    assert game2_prev['data']['name'] == game2_new['data']['name']
    assert game2_prev['data']['maxscore'] == game2_new['data']['maxscore']

def test_games_update_game_organizators(admin_session, game2_uuid):
    """game update organizators"""
    print(test_games_update_game_organizators.__doc__)
    game2_prev = admin_session.game_info({ "uuid": game2_uuid })
    assert game2_prev is not None
    game2_organizators = generate_random(255)
    game2_updt = admin_session.game_update({
        "uuid": game2_uuid,
        "organizators": game2_organizators,
    })
    assert game2_updt is not None
    game2_new = admin_session.game_info({ "uuid": game2_uuid })
    assert game2_new['data']['organizators'] == game2_organizators
    assert game2_prev['data']['description'] == game2_new['data']['description']
    assert game2_prev['data']['name'] == game2_new['data']['name']
    assert game2_prev['data']['maxscore'] == game2_new['data']['maxscore']

def load_image_for_game_logo():
    """ Load image for game logo """
    with open("files/game1.png", 'rb') as _game1_logo:
        image_png_base64 = _game1_logo.read()
        _game1_logo.close()
        img_len = len(image_png_base64)
        assert img_len == 17045
        # if img_len != 17045:
        #     sys.exit("Wrong size of files/game1.png")
        image_png_base64 = base64.b64encode(image_png_base64)
        image_png_base64 = image_png_base64.decode("utf-8")
        return image_png_base64
    return None

def extract_zip_game_data(data_base64, game2_zip_path, tmp_dir_unpack_zip):
    """ extract zip game data """
    game2_data_zip = base64.b64decode(data_base64)
    if os.path.exists(game2_zip_path):
        os.remove(game2_zip_path)
    with open(game2_zip_path, 'wb') as game2_file_zip:
        game2_file_zip.write(game2_data_zip)
        game2_file_zip.close()
        with zipfile.ZipFile(game2_zip_path, 'r') as zip_ref:
            zip_ref.extractall(tmp_dir_unpack_zip)
            zip_ref.close()

def check_game2_logo(img_exported_path, game2_localid, web_server_host):
    """ check game2 logo """
    assert os.path.exists(img_exported_path) is True
    with open(img_exported_path, 'rb') as _game2_logo:
        image2_png_base64 = _game2_logo.read()
        _game2_logo.close()
        img2_len = len(image2_png_base64)
        assert img2_len < 17045 # server must create thumbnail from this image
        url = web_server_host + "public/games/" + str(game2_localid) + ".png"
        resp = requests.get(url, allow_redirects=True)
        open(img_exported_path + "2", 'wb').write(resp.content)
        assert img2_len == len(resp.content)

def test_games_update_logo(admin_session, game2_uuid, local_tmp_dir, web_server_host):
    """game update logo"""
    print(test_games_update_logo.__doc__)
    game2_prev = admin_session.game_info({ "uuid": game2_uuid })
    assert game2_prev is not None
    assert game2_prev["result"] == "DONE"
    image_png_base64 = load_image_for_game_logo()
    game_img = admin_session.game_update_logo({
        "uuid": game2_uuid,
        "image_png_base64": image_png_base64
    })
    assert game_img is not None
    tmp_dir_unpack_zip = local_tmp_dir + '/game2'
    if os.path.isdir(tmp_dir_unpack_zip):
        shutil.rmtree(tmp_dir_unpack_zip)
    game2_export = admin_session.game_export({"uuid": game2_uuid})
    assert game2_export is not None
    assert game2_export["result"] == "DONE"
    assert game2_export['data']['zipfile_name'] == 'game_' + game2_uuid + '.zip'
    game2_zip_path = local_tmp_dir + '/game_' + game2_uuid + '.zip'
    extract_zip_game_data(
        game2_export['data']['zipfile_base64'],
        game2_zip_path,
        tmp_dir_unpack_zip
    )
    json_exported_path = tmp_dir_unpack_zip + '/' + game2_uuid + '.json'
    assert os.path.exists(json_exported_path) is True
    with open(json_exported_path) as _file:
        game2_exported = json.load(_file)
    assert game2_exported["uuid"] == game2_uuid
    assert game2_prev["data"]["name"] == game2_exported["name"]
    assert game2_prev["data"]["description"] == game2_exported["description"]
    assert game2_prev["data"]["maxscore"] == game2_exported["maxscore"]
    assert game2_prev["data"]["organizators"] == game2_exported["organizators"]
    assert game2_prev["data"]["local_id"] ==  game2_exported["local_id"]
    img_exported_path = tmp_dir_unpack_zip + '/' + game2_uuid + '.png'
    check_game2_logo(
        img_exported_path,
        game2_exported["local_id"],
        web_server_host
    )

def test_games_remove(admin_session, admin_password, game2_uuid):
    """game remove"""
    print(test_games_update_logo.__doc__)

    resp = admin_session.game_delete({ "uuid": game2_uuid, "admin_password": admin_password})
    assert resp is not None
    assert resp["result"] == "DONE"

# def test_games_import():
#     """game import """
#     print(test_games_import.__doc__)
    # '''
    # implement import
    # fhqtest.print_bold("Test game import... ")
    # # game1 = admin_session.game_import({ "uuid": game2_uuid });
    # fhqtest.log_warn("Not implemneted yet")
    # implement remove again
    # resp = admin_session.game_delete({ "uuid": game2_uuid, "admin_password": admin_password })
    # fhqtest.check_response(resp, "Game removed")
    # '''
