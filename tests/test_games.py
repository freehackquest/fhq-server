#!/usr/bin/env python
# -*- coding: utf-8 -*-

import libfhqcli
import fhqtest
import sys, traceback
import base64

test_name = 'Testing Games'

try:
    fhqtest.print_header(" > > > " + test_name + ": begin ")
    fhqtest.init_enviroment()

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

    # test create game
    fhqtest.print_bold("Try create game...")
    game2 = fhqtest.admin_session.game_create({
        "uuid": fhqtest.GAME_UUID2,
        "name": "game2",
        "description": "game_description",
        "state": "original", # TODO not has list of states
        "form": "online", # TODO not has list of form
        "type": "jeopardy", # TODO not has list of type
        "date_start": "2000-01-01 00:00:00",
        "date_stop": "2001-01-01 00:00:00",
        "date_restart": "2011-01-01 00:00:00",
        "organizators": "team1, team2"
    })
    fhqtest.alert(game2 == None, 'Could not get response (2)')
    fhqtest.check_response(game2, "Game succesfull created")

    # test find game
    fhqtest.print_bold("Find game by uuid... ")
    game2 = fhqtest.admin_session.game_info({ "uuid": fhqtest.GAME_UUID2 })
    fhqtest.alert(game2 == None, 'Could not get response')
    fhqtest.check_response(game2, "Game found")
    
    fhqtest.print_bold("Check name... ")
    game2 = game2['data']
    if game2['name'] != "game2": # wrong  param name must be name
        fhqtest.log_err("Game has wrong name")
    else:
        fhqtest.print_success("Game name ok")

    gameid = game2['local_id']
    print("gameid: " + str(gameid))

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

    # test update game name
    fhqtest.print_bold("Update game... ")
    game_updt = fhqtest.admin_session.game_update({
        "uuid": fhqtest.GAME_UUID2,
        "name": "game2",
    })
    fhqtest.alert(game_updt == None, 'Could not get response')
    if game_updt['result'] == 'FAIL':
        fhqtest.log_err(game_updt['error'])
    else:
        game1 = fhqtest.admin_session.game_info({ "uuid": fhqtest.GAME_UUID2 })
        fhqtest.print_bold("Check name after update... ")
        fhqtest.check_response(game1, "Games list got")
        game1 = game1['data']
        if game1['name'] != "game2": # wrong  param name must be name
            fhqtest.log_err("Game has wrong name (2)")
        else:
            fhqtest.print_success("Game name ok")

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

    # export
    game_exp = fhqtest.admin_session.game_export({"uuid": fhqtest.GAME_UUID2})
    fhqtest.alert(game_exp == None, 'Could not get response')
    fhqtest.check_response(game_exp, "Game exported")
    game_zip = base64.b64decode(game_exp['data']['zipfile_base64'])
    f = open(fhqtest.TMP_DIR + '/' + game_exp['data']['zipfile_name'], 'wb')
    f.write(game_zip)
    f.close()
    # print(game_exp)

    # remove game
    fhqtest.print_bold("Remove game by uuid... ")
    resp = fhqtest.admin_session.game_delete({ "uuid": fhqtest.GAME_UUID2, "admin_password": fhqtest.admin_password })
    fhqtest.check_response(resp, "Game removed")

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

except Exception as e:
    fhqtest.log_err(str(e))
    traceback.print_exc(file=sys.stdout)
finally:
    fhqtest.print_header(" < < < " + test_name + ": end ")
    fhqtest.deinit_enviroment()
