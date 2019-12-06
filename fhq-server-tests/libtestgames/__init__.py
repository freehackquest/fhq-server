#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from libfreehackquestclient import FreeHackQuestClient
import fhqtest
import sys, traceback
import base64
import os
import zipfile
import shutil
import json
from pprint import pprint

test_name = 'Games'

def run_tests():
    # Cleanup
    fhqtest.print_bold("Clean up test data... ")
    game2 = fhqtest.admin_session.game_info({ "uuid": fhqtest.GAME_UUID2 })
    fhqtest.alert(game2 == None, 'Could not get response')
    if game2['result'] == "DONE":
        game2_delete_r = fhqtest.admin_session.game_delete({ "uuid": fhqtest.GAME_UUID2, "admin_password": fhqtest.ADMIN_PASSWORD})
        fhqtest.alert(game2_delete_r == None, 'Could not get response (game_delete)')
        if game2_delete_r['result'] == 'FAIL':
            fhqtest.log_err("Could not delete prevously game: " + str(game2_delete_r))
            raise ValueError('Could not delete prevously game')
    fhqtest.print_success("Cleaned")

    game_name = fhqtest.generate_random(255)
    game_description = fhqtest.generate_random(300)
    game_teams = fhqtest.generate_random(255)
    game_state = "original"
    game_form = "online"
    game_type = "jeopardy"
    game_date_start = "2000-01-01 00:00:00"
    game_date_stop = "2001-01-01 00:00:00"
    game_date_restart = "2011-01-01 00:00:00"

    # test create game
    fhqtest.print_bold("Try create game...")
    game2 = fhqtest.admin_session.game_create({
        "uuid": fhqtest.GAME_UUID2,
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
    fhqtest.alert(game2 == None, 'Could not get response (2)')
    fhqtest.check_response(game2, "Game succesfull created")

    if game2['result'] == 'FAIL':
        raise ValueError('Could not create game')

    # test find game
    fhqtest.print_bold("Find game by uuid... ")
    game2 = fhqtest.admin_session.game_info({ "uuid": fhqtest.GAME_UUID2 })
    fhqtest.alert(game2 == None, 'Could not get response')
    fhqtest.check_response(game2, "Game found")
    
    game2 = game2['data']

    fhqtest.check_values("name of game", game2['name'], game_name)
    fhqtest.check_values("organizators of game", game2['organizators'], game_teams)
    fhqtest.check_values("form of game", game2['form'], game_form)
    fhqtest.check_values("state of game", game2['state'], game_state)
    fhqtest.check_values("type of game", game2['type'], game_type)
    fhqtest.check_values("description of game", game2['description'], game_description)
    fhqtest.check_values("maxscore of game", game2['maxscore'], 0)

    gameid = game2['local_id']
    # print("gameid: " + str(gameid))

    # list of games
    fhqtest.print_bold("Get list of games... ")
    games_list = fhqtest.admin_session.games({})
    fhqtest.alert(game2 == None, 'Could not get response')
    fhqtest.check_response(games_list, "Games list got")
    bFound = False
    for game_ in games_list['data']:
        if game_['uuid'] == fhqtest.GAME_UUID2:
            bFound = True

    if bFound != True:
        fhqtest.log_err("Game not found in list")
    else:
        fhqtest.print_success("Game found in list")

    '''
    Update game name
    '''
    game_name = fhqtest.generate_random(255)
    fhqtest.print_bold("Update game... ")
    game_updt = fhqtest.admin_session.game_update({
        "uuid": fhqtest.GAME_UUID2,
        "name": game_name,
    })
    fhqtest.alert(game_updt == None, 'Could not get response')
    if game_updt['result'] == 'FAIL':
        fhqtest.log_err(game_updt['error'])
    else:
        game2_new = fhqtest.admin_session.game_info({ "uuid": fhqtest.GAME_UUID2 })
        fhqtest.print_bold("Check name after update... ")
        fhqtest.check_response(game2_new, "Games list got")
        game2_new = game2_new['data']
    
        fhqtest.print_bold("Check the game updated name... ")
        if game2_new['name'] != game_name:
            fhqtest.log_err("Game updated has wrong name expected '" + game_name + "' got '" + game2_new['name'] + "'")
        else:
            fhqtest.print_success("Game the game updated name ok")

        fhqtest.print_bold("Check the game updated organizators... ")
        if game2_new['organizators'] != game_teams:
            fhqtest.log_err("Game updated has wrong organizators expected '" + game_teams + "' got '" + game2_new['organizators'] + "'")
        else:
            fhqtest.print_success("Game the game updated organizators ok")

        fhqtest.print_bold("Check the game updated description... ")
        if game2_new['description'] != game_description:
            fhqtest.log_err("Game updated has wrong description expected '" + game_description + "' got '" + game2_new['description'] + "'")
        else:
            fhqtest.print_success("Game the game updated description ok")

        fhqtest.print_bold("Check the game updated maxscore... ")
        if game2_new['maxscore'] != 0:
            fhqtest.log_err("Game updated has wrong maxscore expected '0' got '" + str(game2_new['maxscore']) + "'")
        else:
            fhqtest.print_success("Game the game updated maxscore ok")

    # TODO upload log for test game
    fhqtest.print_bold("Game update logo test... ")
    f = open("files/game1.png", 'rb')
    image_png_base64 = f.read()
    f.close()
    img_len = len(image_png_base64)
    if img_len != 17045:
        fhqtest.log_err("Game Logo wrong size, current " + str(img_len) + ", but expected 17045")
    image_png_base64 = base64.b64encode(image_png_base64)
    image_png_base64 = image_png_base64.decode("utf-8")
    game_img = fhqtest.admin_session.game_update_logo({
        "uuid": fhqtest.GAME_UUID2,
        "image_png_base64": image_png_base64
    })
    fhqtest.alert(game_img == None, 'Could not get response')
    fhqtest.check_response(game_img, "Game Logo updated")
    
    # cleanup export files
    fhqtest.print_bold("Game cleanup export test... ")
    tmp_dir_unpack_zip = fhqtest.TMP_DIR + '/game2'
    game_zip_path = fhqtest.TMP_DIR + '/game_' + fhqtest.GAME_UUID2 + '.zip'
   
    if os.path.isdir(tmp_dir_unpack_zip):
        shutil.rmtree(tmp_dir_unpack_zip)

    if os.path.exists(game_zip_path):
        os.remove(game_zip_path)

    # export
    fhqtest.print_bold("Game export test... ")
    game_exp = fhqtest.admin_session.game_export({"uuid": fhqtest.GAME_UUID2})
    fhqtest.alert(game_exp == None, 'Could not get response')
    fhqtest.check_response(game_exp, "Game exported")
    if 'game_' + fhqtest.GAME_UUID2 + '.zip' != game_exp['data']['zipfile_name']:
        fhqtest.log_err("zipfile_name for game is wrong ")
    game_zip = base64.b64decode(game_exp['data']['zipfile_base64'])
    f = open(game_zip_path, 'wb')
    f.write(game_zip)
    f.close()

    zip_ref = zipfile.ZipFile(game_zip_path, 'r')
    zip_ref.extractall(tmp_dir_unpack_zip)
    zip_ref.close()

    img_exported_path = tmp_dir_unpack_zip + '/' + fhqtest.GAME_UUID2 + '.png'
    json_exported_path = tmp_dir_unpack_zip + '/' + fhqtest.GAME_UUID2 + '.json'

    if not os.path.exists(img_exported_path):
        fhqtest.log_err("Not found image in zip " + img_exported_path)

    if not os.path.exists(json_exported_path):
        fhqtest.log_err("Not found json in zip " + json_exported_path)
    else:
        with open(json_exported_path) as f:
            game2_exported = json.load(f)
            # pprint(game2_exported)

        fhqtest.print_bold("Check the export game name... ")
        if game2_exported['name'] != game_name:
            fhqtest.log_err("Exported game has wrong name expected '" + game_name + "' got '" + game2_exported['name'] + "'")
        else:
            fhqtest.print_success("Exported game name ok")

        fhqtest.print_bold("Check the export game organizators... ")
        if game2_exported['organizators'] != game_teams:
            fhqtest.log_err("Exported game has wrong organizators expected '" + game_teams + "' got '" + game2_exported['organizators'] + "'")
        else:
            fhqtest.print_success("Exported game organizators ok")

        fhqtest.print_bold("Check the export game description... ")
        if game2_exported['description'] != game_description:
            fhqtest.log_err("Exported game has wrong description expected '" + game_description + "' got '" + game2_exported['description'] + "'")
        else:
            fhqtest.print_success("Exported game description ok")

        fhqtest.print_bold("Check the export game maxscore... ")
        if game2_exported['maxscore'] != 0:
            fhqtest.log_err("Exported game has wrong maxscore expected '0' got '" + str(game2_exported['maxscore']) + "'")
        else:
            fhqtest.print_success("Exported game maxscore ok")

        gameid = game2_exported['local_id']

    # print(game_exp)

    # remove game
    fhqtest.print_bold("Remove game by uuid... ")
    resp = fhqtest.admin_session.game_delete({ "uuid": fhqtest.GAME_UUID2, "admin_password": fhqtest.admin_password })
    fhqtest.check_response(resp, "Game removed")

    fhqtest.check_response(resp, "TODO Game import")

    '''
    # TODO import
    fhqtest.print_bold("Test game import... ")
    # game1 = fhqtest.admin_session.game_import({ "uuid": fhqtest.GAME_UUID2 });
    fhqtest.log_warn("Not implemneted yet")

    # TODO remove again
    fhqtest.print_bold("Remove game by uuid... ")
    resp = fhqtest.admin_session.game_delete({ "uuid": game_uuid1, "admin_password": fhqtest.admin_password })
    fhqtest.check_response(resp, "Game removed")
    '''

