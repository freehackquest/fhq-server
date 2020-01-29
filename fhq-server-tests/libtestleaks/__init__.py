#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from libfreehackquestclient import FreeHackQuestClient
import fhqtest
import sys, traceback
import uuid
import libtestleaks

test_name = 'Testing Leaks'

def run_tests():
	leak_uuid1 = uuid.uuid1()
	leak1 = None

    #cleanup
	fhqtest.print_bold("Clean up test data... ")
	leaks = fhqtest.admin_session.leaks_list({"page": 0, "onpage": 10})
	for leak in leaks["data"]:
		fhqtest.admin_session.leaks_delete({"id": leak["id"]})
		print("Leak with id #" + str(leak["id"]) + " was succesfully deleted!")

	fhqtest.print_success("Cleaned")

    #leak creation test
	fhqtest.print_bold("Try create leak...")
	leak1 = fhqtest.admin_session.leaks_add({
		"uuid": str(leak_uuid1),
		"game_uuid": fhqtest.GAME_UUID1,
		# "gameid": 0,
		"name": "test_leak1",
		"content": "some_secret_content",
		"score": 100
	})
	fhqtest.alert(leak1 == None, 'Could not get response')
	fhqtest.check_response(leak1, "Leak succesfull created")

	#leak update test
	leak_id = None
	leaks = fhqtest.admin_session.leaks_list({"page": 0, "onpage": 10})
	for leak in leaks["data"]:
		if leak["id"] != '':
			leak_id = leak["id"]
		else:
			fhqtest.log_err("Id could not be empty")

	fhqtest.print_bold("Update leak... ")
	leak_updt  = fhqtest.admin_session.leaks_update({
		"id": leak_id,
		"name": "test_leak1_new",
	})
	fhqtest.alert(leak_updt == None, 'Could not get response')
	if leak_updt['result'] == 'FAIL':
		fhqtest.log_err(leak_updt['error'])
	else:
		leaks = fhqtest.admin_session.leaks_list({"page": 0, "onpage": 10})
		for leak in leaks["data"]:
			if leak["id"] == leak_id:
				fhqtest.print_bold("Check name after update... ")
				if leak["name"] != "test_leak1_new":
					fhqtest.log_err("Leak has wrong name")
				else:
					fhqtest.print_success("Leak has been successfully updated")
			else:
				fhqtest.log_err("This leak isn't the one we updated!")
				print("Id of this leak is #" + str(leak["id"]))
	
	#TODO leaks_list test
	
    #TODO leaks_buy test