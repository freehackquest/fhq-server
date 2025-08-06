#!/usr/bin/env python3
# Copyright (C) 2011-2025, Evgenii Sopov <mrseakg@gmail.com>

# pylint: disable=consider-using-with

""" Configuration for pytest """

import os
import subprocess
import time
import signal
import socket
import random
import string
import pytest
import docker

from freehackquest_libclient_py import FreeHackQuestClient

def pytest_configure():
    """Global variables for pytests"""
    pytest.something = "some"
    pytest.server_started = False
    pytest.popen_fhq_server = None
    pytest.admin_session = None
    pytest.admin_email = "admin"
    pytest.admin_password = "admin"
    pytest.test_host = "127.0.0.1"
    pytest.test_server = "ws://" + pytest.test_host + ":1234/"
    pytest.test_url_http_web_server = "http://" + pytest.test_host + ":7080/"
    pytest.game1_uuid = "00000000-0000-0000-1000-000000000001"
    pytest.game2_uuid = "00000000-0000-0000-1000-000000000002"
    pytest.user1_uuid = "00000000-0000-0000-4001-000000000001"
    pytest.user2_uuid = "00000000-0000-0000-4001-000000000002"
    pytest.user3_uuid = "00000000-0000-0000-4001-000000000003"
    pytest.user1_data = {
        "uuid": pytest.user1_uuid,
        "email": "user1",
        "nick": "nick_user1",
        "password": "user1",
        "role": "user"
    }
    pytest.user2_data = {
        "uuid": pytest.user2_uuid,
        "email": "user2",
        "nick": "nick_user2",
        "password": "user2",
        "role": "user"
    }
    pytest.user3_data = {
        "uuid": pytest.user3_uuid,
        "email": "user3",
        "nick": "nick_user3",
        "password": "user3",
        "role": "user"
    }
    pytest.user3_data_again = {
        "uuid": pytest.user3_uuid,
        "email": "user3",
        "nick": "nick_user3_again",
        "password": "user3",
        "role": "user"
    }

@pytest.fixture(scope="session")
def local_tmp_dir():
    """tmp directory for tests"""
    tmp_dir_path = "./tmp"
    if not os.path.isdir(tmp_dir_path):
        os.mkdir(tmp_dir_path)
    return tmp_dir_path

@pytest.fixture(scope="session")
def url_http_web_server():
    """ web_server_http """
    return pytest.test_url_http_web_server

def _generate_random(size):
    """ Generate random printable string """
    _range = range(size)
    _alphabet = string.ascii_uppercase + string.digits + ' _+=\'"~@!#?/<>'
    return ''.join(random.choice(_alphabet) for _ in _range)

@pytest.fixture(scope="session")
def generate_random():
    """ generate_random """
    return _generate_random

def _generate_random_uuid():
    """ _generate_random_uuid """
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

@pytest.fixture(scope="session")
def generate_random_uuid():
    """ generate_random_uuid """
    return _generate_random_uuid

@pytest.fixture(scope="session")
def admin_session():
    """Return admin_session"""
    if pytest.admin_session is None:
        pytest.admin_session = FreeHackQuestClient(pytest.test_server)
    resp = pytest.admin_session.login({
        "email": pytest.admin_email,
        "password": pytest.admin_password
    })
    assert resp is not None
    assert resp['result'] != 'FAIL'

     # loggined = True
    game1 = pytest.admin_session.game_info({"uuid": pytest.game1_uuid})
    assert game1 is not None
    if game1['result'] == 'FAIL':
        game1 = pytest.admin_session.game_create({
            "uuid": pytest.game1_uuid,
            "name": "test",
            "freehackquest_game_format_version": 1,
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
    return pytest.admin_session

@pytest.fixture(scope="session")
def admin_password():
    """Return admin_password"""
    return pytest.admin_password

@pytest.fixture(scope="session")
def user1_data():
    """Return user1_data"""
    return pytest.user1_data

@pytest.fixture(scope="session")
def user2_data():
    """Return user2_data"""
    return pytest.user2_data

@pytest.fixture(scope="session")
def user3_data():
    """Return user3_data"""
    return pytest.user3_data

@pytest.fixture(scope="session")
def user3_data_again():
    """Return user3_data"""
    return pytest.user3_data_again

@pytest.fixture(scope="session")
def test_server():
    """Return web server host"""
    return pytest.test_server

@pytest.fixture(scope="session")
def game1_uuid():
    """Return game1 uuid"""
    return pytest.game1_uuid

@pytest.fixture(scope="session")
def game2_uuid():
    """Return game2 uuid"""
    return pytest.game2_uuid

@pytest.fixture(scope="session")
def classbook_record1_uuid():
    """Return classbook record1 uuid"""
    return "C1A55800-0000-0000-0000-000000000001"

@pytest.fixture(scope="session")
def classbook_record2_uuid():
    """Return classbook record2 uuid"""
    return "C1A55800-0000-0000-0000-000000000002"

@pytest.fixture(scope="session")
def classbook_record3_uuid():
    """Return classbook record3 uuid"""
    return "C1A55800-0000-0000-0000-000000000003"

@pytest.fixture(scope="session")
def classbook_record4_uuid():
    """Return classbook record4 uuid"""
    return "C1A55800-0000-0000-0000-000000000004"

# QUEST_UUID1 = "03E51000-0000-0000-0000-000000000001"
# QUEST_UUID2 = "03E51000-0000-0000-0000-000000000002"
# QUEST_UUID3 = "03E51000-0000-0000-0000-000000000003"
# QUEST_UUID4 = "03E51000-0000-0000-0000-000000000004"

@pytest.fixture(scope="session", autouse=True)
def callattr_ahead_of_alltests(): # request
    """ Start server before tests and after tests """
    print("Before tests")
    assert start_server() is True
    yield
    print("\nAfter tests")
    stop_server()

def check_port(host, port):
    """Check socket port"""
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    result = sock.connect_ex((host, port))
    sock.close()
    return result == 0

def start_server():
    """Start fhq-server"""
    if pytest.server_started:
        print("Server already started")
        return True
    env_start_server = os.environ.get("START_SERVER")
    print("Starting server " + str(env_start_server))
    if env_start_server == 'local':
        pytest.server_started = True
        return start_local_server()
    if env_start_server == 'docker':
        pytest.server_started = True
        return start_server_in_docker()
    print("START_SERVER can be 'local' or 'docker'")
    return False

def stop_server():
    """Stop fhq-server"""
    if not pytest.server_started:
        print("Server did not started")
        return
    env_start_server = os.environ.get("START_SERVER")
    print("Stop server " + str(env_start_server))
    if env_start_server == 'local':
        stop_local_server()
        pytest.server_started = False
    elif env_start_server == 'docker':
        stop_server_in_docker()
        pytest.server_started = False
    else:
        print("START_SERVER can be 'local' or 'docker'")

def start_local_server():
    """Start process of fhq-server"""
    print("Start fhq-server from local binaries")
    current_wor_dir = os.getcwd()
    # print(current_wor_dir)
    # os.chdir(current_wor_dir + "/../../fhq-server")
    cmd = [current_wor_dir + '/../../fhq-server', '-wd', '../../ci/travis/data', 'start']
    pytest.popen_fhq_server = subprocess.Popen(cmd)
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
    if pytest.popen_fhq_server is not None:
        print("Kill process " + str(pytest.popen_fhq_server.pid))
        os.kill(pytest.popen_fhq_server.pid, signal.SIGKILL)


# prepare folders with config
def prepare_files_for_start_in_docker():
    """Prepare dirs and files for start fhq-server in docker"""
    os.makedirs("tmp_docker/data/conf.d", exist_ok=True)
    os.makedirs("tmp_docker/data/logs", exist_ok=True)
    os.makedirs("tmp_docker/data/fhqjad-store", exist_ok=True)
    os.makedirs("tmp_docker/data/public/games", exist_ok=True)
    if not os.path.exists("tmp_docker/mysql"):
        os.makedirs("tmp_docker/mysql", exist_ok=True)
    os.makedirs("tmp_docker/data/public/quests", exist_ok=True)
    fhq_server_conf_path = "tmp_docker/data/conf.d/fhq-server.conf"
    with open(fhq_server_conf_path, "w", encoding="utf-8") as fhq_server_conf:
        fhq_server_conf.write("""
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
web_user_folder = /usr/share/fhq-server/web-user/

web_fhqjad_store = /usr/share/fhq-server/fhqjad-store/

""")
    test_selfsigned_key_path = "tmp_docker/data/conf.d/test-selfsigned.key"
    with open(test_selfsigned_key_path, "w", encoding="utf-8") as test_selfsigned_key:
        test_selfsigned_key.write("")
    test_selfsigned_crt_path = "tmp_docker/data/conf.d/test-selfsigned.crt"
    with open(test_selfsigned_crt_path, "w", encoding="utf-8") as test_selfsigned_crt:
        test_selfsigned_crt.write("")

# prepare folders with config
def start_mysql_for_start_in_docker():
    """Start mysql container for start fhq-server in docker"""

    client = docker.from_env()
    containers = client.containers.list(all=True)
    found = False
    for container in containers:
        if container.name == "pytest-fhq-server-db":
            found = True
    if not found:
        dir_mysql = os.getcwd() + "/tmp_docker/mysql"
        mount_mysql = docker.types.Mount(
            target="/var/lib/mysql",
            source=dir_mysql,
            type="bind"
        )
        client.containers.run(
            "mysql:5.7",
            mem_limit="128M",
            memswap_limit="128M",
            auto_remove=True,
            mounts=[mount_mysql],
            ports={"3306/tcp": 3706},
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
        # """
        # simular command:
        # docker run --rm \
        #     -e MYSQL_ROOT_PASSWORD=fhqroot \
        #     -e MYSQL_DATABASE=fhq \
        #     -e MYSQL_USER=fhq \
        #     -e MYSQL_PASSWORD=fhq \
        #     --memory 128MB --memory-swap 128MB \
        #     -p "3706:3306" \
        #     -v `pwd`//tmp_docker/mysql:/var/lib/mysql \
        #     --name "pytest-fhq-server-db" \
        #     mysql:5.7
        # """

# prepare folders with config
def try_start_server_for_start_in_docker():
    """Try start fhq-server container for start fhq-server in docker"""
    client = docker.from_env()
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
    containers = client.containers.list(all=True)
    found = False
    for container in containers:
        if container.name == "pytest-fhq-server":
            found = True
    if not found:
        client.containers.run(
            "freehackquest/fhq-server:latest",
            mem_limit="128M",
            memswap_limit="128M",
            auto_remove=True,
            mounts=[mount_fhq_conf, dir_public, dir_store, dir_logs],
            ports={"1234/tcp": 1234, "7080/tcp": 7080},
            name="pytest-fhq-server",
            detach=True,
            network_mode="host",
        )
        # """
        # simular command:
        #     docker run --rm \
        #     --memory 128MB --memory-swap 128MB \
        #     -p "1234:1234" -p "7080:7080" \
        #     -v `pwd`/tmp_docker/data/conf.d:/etc/fhq-server \
        #     -v `pwd`/tmp_docker/data/public:/usr/share/fhq-server/fhq-web-public \
        #     -v `pwd`/tmp_docker/data/logs:/var/log/fhq-server \
        #     -v `pwd`/tmp_docker/data/fhqjad-store:/usr/share/fhq-server/fhqjad-store \
        #     --name "pytest-fhq-server" \
        #     --network host \
        #     freehackquest/fhq-server:latest
        # """

def start_server_in_docker():
    """Start fhq-server in docker"""

    # stop_server_in_docker()

    prepare_files_for_start_in_docker()
    start_mysql_for_start_in_docker()

    wait_max = 20
    wait_i = 0
    result_check_port = False
    while wait_i < wait_max:
        wait_i = wait_i + 1
        try_start_server_for_start_in_docker()
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
    containers = client.containers.list(all=True)
    for container in containers:
        if container.name == "pytest-fhq-server":
            print("Stopping container " + container.name)
            container.stop()
            print("Removing container " + container.name)
    for container in containers:
        if container.name == "pytest-fhq-server-db":
            print("Stopping container " + container.name)
            container.stop()
            print("Removing container " + container.name)
