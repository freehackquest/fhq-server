#!/usr/bin/env python3
# -*- coding: utf-8 -*-
""" Configuration for pytest """

import os
import subprocess
import time
import signal
import socket
import pytest
from libfreehackquestclient import FreeHackQuestClient

ADMIN_EMAIL = "admin"
ADMIN_PASSWORD = "admin"
TEST_HOST = "127.0.0.1"
TEST_SERVER = "ws://" + TEST_HOST + ":1234/"
TEST_WEB_SERVER = "http://" + TEST_HOST + ":7080/"
TMP_DIR = "./tmp"
ADMIN_SESSION = None
POPEN_FHQ_SERVER = None

def check_port(host, port):
    """Check socket port"""
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    result = sock.connect_ex((host, port))
    return result == 0

def start_server():
    """Start process of fhq-server"""
    print("Start fhq-server")
    current_wor_dir = os.getcwd()
    # print(current_wor_dir)
    # os.chdir(current_wor_dir + "/../../fhq-server")
    cmd = [current_wor_dir + '/../../fhq-server/fhq-server', '-wd', '../../ci/travis/data', 'start']
    global POPEN_FHQ_SERVER
    POPEN_FHQ_SERVER = subprocess.Popen(cmd)
    # os.chdir(current_wor_dir)

    wait_max = 20
    wait_i = 0
    result_check_port = False
    while wait_i < wait_max:
        wait_i = wait_i + 1
        time.sleep(1)
        result_check_port = check_port('127.0.0.1', 1234)
        if not result_check_port:
            print(" =====> " + str(wait_i) + ": port not available... ")
        else:
            break

    if not result_check_port:
        print("Port not available... failed")
        return False
    print("Port available... OK!")
    return True

def stop_server():
    """Stop process of fhq-server"""
    print("Stop fhq-server")
    if POPEN_FHQ_SERVER is not None:
        print("Kill process " + str(POPEN_FHQ_SERVER.pid))
        os.kill(POPEN_FHQ_SERVER.pid, signal.SIGKILL)

@pytest.fixture(scope="module")
def admin_session():
    """Return admin_session"""
    session = FreeHackQuestClient(TEST_SERVER)
    resp = session.login({"email": ADMIN_EMAIL, "password": ADMIN_PASSWORD})
    assert resp is not None
    assert resp['result'] != 'FAIL'
    return session

@pytest.fixture(scope="module")
def game1_uuid():
    """Return admin_session"""
    return "00000000-0000-0000-1000-000000000001"

@pytest.fixture(scope="session", autouse=True)
def run_around_tests():
    """Prepare evn for tests"""
    # before tests
    assert start_server() is True
    ADMIN_SESSION = FreeHackQuestClient(TEST_SERVER)
    # run tests
    yield
    # after tests
    if ADMIN_SESSION is not None:
        ADMIN_SESSION.close()
    stop_server()
