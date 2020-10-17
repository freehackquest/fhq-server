#!/usr/bin/env python3
# -*- coding: utf-8 -*-
""" Configuration for pytest """

import os
import subprocess
import time
import signal
import socket
import pytest
import docker

from libfreehackquestclient import FreeHackQuestClient

ADMIN_EMAIL = "admin"
ADMIN_PASSWORD = "admin"
TEST_HOST = "127.0.0.1"
TEST_SERVER = "ws://" + TEST_HOST + ":1234/"
TEST_WEB_SERVER = "http://" + TEST_HOST + ":7080/"
TMP_DIR = "./tmp"
ADMIN_SESSION = None
POPEN_FHQ_SERVER = None
STARTED_SERVER = False

@pytest.fixture(scope="session")
def admin_session():
    """Return admin_session"""
    session = FreeHackQuestClient(TEST_SERVER)
    resp = session.login({"email": ADMIN_EMAIL, "password": ADMIN_PASSWORD})
    assert resp is not None
    assert resp['result'] != 'FAIL'

     # loggined = True
    game1_uuid = "00000000-0000-0000-1000-000000000001"
    game1 = session.game_info({"uuid": game1_uuid})
    assert game1 is not None
    if game1['result'] == 'FAIL':
        game1 = session.game_create({
            "uuid": game1_uuid,
            "name": "test",
            "description": "test",
            "state": "original",
            "form": "online",
            "type": "jeopardy",
            "date_start": "2000-01-01 00:00:00",
            "date_stop": "2001-01-01 00:00:00",
            "date_restart": "2002-01-01 00:00:00",
            "organizators": "test"
        })
        assert game1 is not None
        assert game1['result'] == 'DONE'
    return session

@pytest.fixture(scope="session")
def game1_uuid():
    """Return admin_session"""
    return "00000000-0000-0000-1000-000000000001"


@pytest.fixture(scope="session", autouse=True)
def callattr_ahead_of_alltests(request):
    print("Before tests")
    assert start_server() is True
    yield
    print("After tests")
    stop_server()

def check_port(host, port):
    """Check socket port"""
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    result = sock.connect_ex((host, port))
    return result == 0

def start_server():
    """Start fhq-server"""
    global STARTED_SERVER
    if STARTED_SERVER == True:
        print("Server already started")
        return True
    env_start_server = os.environ.get("START_SERVER")
    print("Starting server " + str(env_start_server))
    if env_start_server == 'local':
        STARTED_SERVER = True
        return start_local_server()
    elif env_start_server == 'docker':
        STARTED_SERVER = True
        return start_server_in_docker()
    print("START_SERVER can be 'local' or 'docker'")
    return False

def stop_server():
    """Stop fhq-server"""
    global STARTED_SERVER
    if STARTED_SERVER == False:
        print("Server did not started")
        return
    env_start_server = os.environ.get("START_SERVER")
    print("Stop server " + str(env_start_server))
    if env_start_server == 'local':
        stop_local_server()
        STARTED_SERVER = False
    elif env_start_server == 'docker':
        stop_server_in_docker()
        STARTED_SERVER = False
    else:
        print("START_SERVER can be 'local' or 'docker'")

def start_local_server():
    """Start process of fhq-server"""
    print("Start fhq-server from local binaries")
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

def stop_local_server():
    """Stop process of fhq-server"""
    print("Stop fhq-server from local binaries")
    if POPEN_FHQ_SERVER is not None:
        print("Kill process " + str(POPEN_FHQ_SERVER.pid))
        os.kill(POPEN_FHQ_SERVER.pid, signal.SIGKILL)


def start_server_in_docker():
    """Start fhq-server in docker"""

    # stop_server_in_docker()

    # prepare folders with config
    os.makedirs("tmp_docker/data/conf.d", exist_ok=True)
    os.makedirs("tmp_docker/data/logs", exist_ok=True)
    os.makedirs("tmp_docker/data/fhqjad-store", exist_ok=True)
    os.makedirs("tmp_docker/data/public/games", exist_ok=True)
    os.makedirs("tmp_docker/mysql", exist_ok=True)
    os.makedirs("tmp_docker/data/public/quests", exist_ok=True)
    with open("tmp_docker/data/conf.d/fhq-server.conf", "w") as f:
        f.write("""
storage_type = mysql
dbhost = 127.0.0.1
dbname = fhq
dbport = 3706
dbuser = fhq
dbpass = fhq

## Server configurations
port = 1234
ssl_on = no
ssl_port = 4613
ssl_key_file = ./test-selfsigned.key
ssl_cert_file = ./test-selfsigned.crt

# Web Configuration
web_port = 7080
web_max_threads = 1

web_admin_folder = /usr/share/fhq-server/web-admin/
web_public_folder = /usr/share/fhq-server/fhq-web-public/
web_public_folder_url = http://localhost:7080/public/
web_user_folder = /usr/share/fhq-server/fhq-web-user/

web_fhqjad_store = /usr/share/fhq-server/fhqjad-store/

""")

    with open("tmp_docker/data/conf.d/test-selfsigned.key", "w") as f:
        f.write("")
    with open("tmp_docker/data/conf.d/test-selfsigned.crt", "w") as f:
        f.write("")


    client = docker.from_env()

    dir_mysql = os.getcwd() + "/tmp_docker/mysql"
    mount_mysql = docker.types.Mount(
        target="/var/lib/mysql",
        source=dir_mysql,
        type="bind"
    )

    cntrs = client.containers.list(all=True)
    found = False
    for c in cntrs:
        if c.name == "pytest-fhq-server-db":
            found = True
    if not found:
        client.containers.run(
            "mysql:5.7",
            mem_limit="128M",
            memswap_limit="128M",
            auto_remove=True,
            mounts=[mount_mysql],
            ports={ "3306/tcp": 3706 },
            name="pytest-fhq-server-db",
            detach=True,
            environment={
                "MYSQL_ROOT_PASSWORD": "fhqroot",
                "MYSQL_DATABASE": "fhq",
                "MYSQL_USER": "fhq",
                "MYSQL_PASSWORD": "fhq",
            },
            command="--default-authentication-plugin=mysql_native_password"
        )

        """
        simular command:
        docker run --rm \
            -e MYSQL_ROOT_PASSWORD=fhqroot \
            -e MYSQL_DATABASE=fhq \
            -e MYSQL_USER=fhq \
            -e MYSQL_PASSWORD=fhq \
            --memory 128MB --memory-swap 128MB \
            -p "3706:3306" \
            -v `pwd`//tmp_docker/mysql:/var/lib/mysql \
            --name "pytest-fhq-server-db" \
            mysql:5.7
        """

    dir_conf = os.getcwd() + "/tmp_docker/data/conf.d"
    dir_public = os.getcwd() + "/tmp_docker/data/public"
    dir_store = os.getcwd() + "/tmp_docker/data/fhqjad-store"
    dir_logs = os.getcwd() + "/tmp_docker/data/logs"
    mount_fhq_conf = docker.types.Mount(
        target="/etc/fhq-server",
        source=dir_conf,
        type="bind"
    )
    dir_public = docker.types.Mount(
        target="/usr/share/fhq-server/fhq-web-public",
        source=dir_public,
        type="bind"
    )
    dir_store = docker.types.Mount(
        target="/usr/share/fhq-server/fhqjad-store",
        source=dir_store,
        type="bind"
    )

    dir_logs = docker.types.Mount(
        target="/var/log/fhq-server",
        source=dir_logs,
        type="bind"
    )

    wait_max = 20
    wait_i = 0
    result_check_port = False
    while wait_i < wait_max:
        wait_i = wait_i + 1
        cntrs = client.containers.list(all=True)
        found = False
        for c in cntrs:
            if c.name == "pytest-fhq-server":
                found = True
        if not found:
            client.containers.run(
                "freehackquest/fhq-server:latest",
                mem_limit="128M",
                memswap_limit="128M",
                auto_remove=True,
                mounts=[mount_fhq_conf, dir_public, dir_store, dir_logs],
                ports={ "1234/tcp": 1234, "7080/tcp": 7080 },
                name="pytest-fhq-server",
                detach=True,
                network_mode="host",
            )
            """
            simular command:
                docker run --rm \
                --memory 128MB --memory-swap 128MB \
                -p "1234:1234" -p "7080:7080" \
                -v `pwd`/tmp_docker/data/conf.d:/etc/fhq-server \
                -v `pwd`/tmp_docker/data/public:/usr/share/fhq-server/fhq-web-public \
                -v `pwd`/tmp_docker/data/logs:/var/log/fhq-server \
                -v `pwd`/tmp_docker/data/fhqjad-store:/usr/share/fhq-server/fhqjad-store \
                --name "pytest-fhq-server" \
                --network host \
                freehackquest/fhq-server:latest
            """
        time.sleep(1)
        result_check_port = check_port('127.0.0.1', 1234)
        if not result_check_port:
            print(" =====> " + str(wait_i) + ": port not available... ")
        else:
            print(" =====> " + str(wait_i) + ": port available!!! ")
            break

    return result_check_port

def stop_server_in_docker():
    """Stop fhq-server in docker"""
    client = docker.from_env()
    cntrs = client.containers.list(all=True)
    for c in cntrs:
        if c.name == "pytest-fhq-server":
            print("Stopping container " + c.name)
            c.stop()
            print("Removing container " + c.name)
    for c in cntrs:
        if c.name == "pytest-fhq-server-db":
            print("Stopping container " + c.name)
            c.stop()
            print("Removing container " + c.name)
