# Tests for fhq-server

## Developer info

### Refresh libfhqcli library

This operation will be need when changed some logic for handlers

```
$ cd ~/develop/freehackquest/backend.git
$ cd fhq-server
$ ./build_simple.sh
$ ./fhq-server -eclp # export python library
$ cp -rf libfhqcli-py/libfhqcli/ ../tests/
```

### Look which users will need for tests

Information for tests:

```
ADMIN_EMAIL = "admin"
ADMIN_PASSWORD = "admin"
TEST_SERVER = "ws://localhost:1234/"
```

Or more informartion you can find in file `fhqtest.py`

## Run use python2

```
$ cd ~/develop/freehackquest/backend.git
$ cd tests/
$ pip install -r requirements.txt
$ python2 test_games.py
```

## Run use python3 

```
$ cd ~/develop/freehackquest/backend.git
$ cd tests/
$ pip install -r requirements.txt
$ python3 test_games.py
```
