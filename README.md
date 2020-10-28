# FHQ Server (backend + frontend)

[![Build Status](https://travis-ci.org/freehackquest/fhq-server.svg?branch=master)](https://travis-ci.org/freehackquest/fhq-server) [![Docker Pulls](https://img.shields.io/docker/pulls/freehackquest/fhq-server.svg)](https://hub.docker.com/r/freehackquest/fhq-server/) [![Docker layers](https://images.microbadger.com/badges/image/freehackquest/fhq-server.svg)](https://microbadger.com/images/freehackquest/fhq-server) [![Github Stars](https://img.shields.io/github/stars/freehackquest/fhq-server.svg?label=github%20%E2%98%85)](https://github.com/freehackquest/fhq-server/) [![Github Stars](https://img.shields.io/github/contributors/freehackquest/fhq-server.svg)](https://github.com/freehackquest/fhq-server/) [![Github Forks](https://img.shields.io/github/forks/freehackquest/fhq-server.svg?label=github%20forks)](https://github.com/freehackquest/fhq-server/) [![Total alerts](https://img.shields.io/lgtm/alerts/g/freehackquest/fhq-server.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/freehackquest/fhq-server/alerts/) [![Language grade: Python](https://img.shields.io/lgtm/grade/python/g/freehackquest/fhq-server.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/freehackquest/fhq-server/context:python) [![Language grade: JavaScript](https://img.shields.io/lgtm/grade/javascript/g/freehackquest/fhq-server.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/freehackquest/fhq-server/context:javascript) [![deepcode](https://www.deepcode.ai/api/gh/badge?key=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwbGF0Zm9ybTEiOiJnaCIsIm93bmVyMSI6ImZyZWVoYWNrcXVlc3QiLCJyZXBvMSI6ImZocS1zZXJ2ZXIiLCJpbmNsdWRlTGludCI6ZmFsc2UsImF1dGhvcklkIjoxNTY0MSwiaWF0IjoxNjAxNTQ5MzE2fQ.UBFXoYtnCuMZfzi2pD51K4m46MSVwD8yvqgI0EMoEjY)](https://www.deepcode.ai/app/gh/freehackquest/fhq-server/_/dashboard?utm_content=gh%2Ffreehackquest%2Ffhq-server)


Backend && Frontend for FreeHackQuest on Qt and WebSockets

**!!! FAST START YOUR GAME** [here (based on docker-compose)](https://github.com/freehackquest/freehackquest-start-game)!!!

* [HOW-TO-BUILD-AND-DEVELOP](https://github.com/freehackquest/fhq-server/tree/master/install/HOW-TO-BUILD-AND-DEVELOP.md)
* [PRE-BUILDS](https://github.com/freehackquest/fhq-server/tree/master/install/PRE-BUILDS.md)
* [INSTALL_FROM_PPA](https://github.com/freehackquest/fhq-server/tree/master/install/INSTALL_FROM_PPA.md)

* Configure autostart
    * [SYSTEMD](install/SYSTEMD.md)
    * [INITD](install/INITD.md)

## Distribution

| What                    | Where         |
| ----------------------- | ------------- |
| fhq-server (docker)     | [![Docker Pulls](https://img.shields.io/docker/pulls/freehackquest/fhq-server.svg)](https://hub.docker.com/r/freehackquest/fhq-server/)  |
| fhq-server (vm)         | [https://freehackquest.com/dist/](https://freehackquest.com/dist/)  |
| client library (python) | [![npm version](https://badge.fury.io/js/freehackquest-libclient-web-js.svg)](https://badge.fury.io/js/freehackquest-libclient-web-js) |
| client library (js)     | [![PyPI version](https://badge.fury.io/py/libfreehackquestclient.svg)](https://badge.fury.io/py/libfreehackquestclient) |


## For developers

* [Doxygen Documentation](https://freehackquest.com/doxygen/)

### Build c++ server

#### Ubuntu/Debian

Requirements:

```
$ sudo apt install git-core g++ make cmake qtchooser qt5-default libqt5websockets5 libqt5websockets5-dev libqt5sql5-mysql
```
New Requirements

```
$ sudo apt install libwebsockets-dev libcurl4-openssl-dev
$ sudo apt install zlibc zlib1g zlib1g-dev
$ sudo apt install libpng-dev
$ sudo apt install libmysqlclient-dev
```

Clone repository:

```
$ git clone https://github.com/freehackquest/fhq-server ~/fhq-server.git
```

Build:

```
$ cd ~/fhq-server.git/fhq-server
$ ./build_simple.sh
./fhq-server -wd ../ci/travis/data start
```

Build and run unit-tests:

```
$ cd ~/fhq-server.git/fhq-server/unit-tests.wsjcpp
$ ./build_simple.sh
$ ./unit-tests
```

### Server api tests

Based on python3 and pytest. Also please check code by pylint.

Install Requirements:

```
$ sudo apt install python3-pip 
$ pip3 install websocket-client requests pytest pytest-env docker pylint
```

Exists two ways:
1. When using a fresh compiled fhq-server on local machine
2. Use a docker image latest version from hub.docker.com (here: https://hub.docker.com/r/freehackquest/fhq-server)

#### Server api tests - first way (local binary mode)

Expected that repository already got and fhq-server compiled. Also database configured for ci/travis/data.

Run tests (will be deprecated):

```
$ cd ~/fhq-server.git/fhq-server-tests
$ ./update_libfreehackquestclient.sh # update auto-generate-client-library
$ python3 run_tests.py # run tests
```

New running tests (based on pytest):

```
$ cd ~/fhq-server.git/tests/server-api-tests/
$ ./update_libfreehackquestclient.sh # update auto-generate-client-library
$ python3 -m pylint --rcfile=.pylintrc *.py
$ python3 -m pytest -rAs -c env-travis.ini .
```

### Server api tests - second way (docker mode)

Since you do not have a local server, you need to install the client library:

```
$ pip3 install libfreehackquestclient
```

Donwload docker images:
```
$ docker pull freehackquest/fhq-server
$ docker pull mysql:5.7
```

And now you can try run server-api-tests

```
$ cd ~/fhq-server.git/tests/server-api-tests
$ python3 -m pylint --rcfile=.pylintrc *.py
$ python3 -m pytest -rAs -c env-docker.ini .
```

### Web User Interface

Fast to fix and develop for current server version

```
$ git clone git@github.com:freehackquest/fhq-server.git ~/fhq-server.git
$ cd ~/fhq-server.git/fhq-web-user
$ npm install 
$ npm run start
```

In a file `~/fhq-server.git/fhq-web-user/src/app/services/fhq.service.ts`
And then you can just uncomment line:
```
// baseUrl = 'ws://freehackquest.com/api-ws/';
```
to 
```
baseUrl = 'ws://freehackquest.com/api-ws/';
```

And now your local web site will be connected to offical server.

*Notice: but please never do commit for this line*


## Development

### Method which will return lists 

fhq-server input api:
```cpp
optionalIntegerParam("page_size", "Pgae size")
    .addValidator(new WsjcppValidatorIntegerMinValue(0))
    .addValidator(new WsjcppValidatorIntegerMinValue(10));
optionalIntegerParam("page_index", "Page index")
    .addValidator(new WsjcppValidatorIntegerMinValue(0));
```

fhq-server output api:
```
nlohmann::json jsonResult;
jsonResult["items"] = jsonItems;
jsonResult["page_size"] = nPageSize;
jsonResult["page_index"] = nPageIndex;
jsonResult["total"] = nTotal;

nlohmann::json jsonResponse;
jsonResponse["data"] = jsonResult;
pRequest->sendMessageSuccess(cmd(), jsonResponse);
```

admin-web-site paginator:
```
var page_name = 'quests_proposal';
var pg = new SwaPaginator(0, r.data.total, r.data.page_size, r.data.page_index);
el.append(pg.getHtml());
pg.bindPrev(function() {
    window.fhq.changeLocationState({page_name: '', 'page_size': page_size, 'page_index': page_index - 1});
    fhq.pages[page_name]();
});

pg.bindNext(function() {
    window.fhq.changeLocationState({page_name: '', 'page_size': page_size, 'page_index': page_index + 1});
    fhq.pages[page_name]();
});
```


## 3rdParty

* C++ Source Code Managment: [wsjcpp](https://wsjcpp.org)
* For Json: [nlohmann::json](https://github.com/nlohmann/json)
* Used libcurl for a working with http/https/smtp: [https://curl.haxx.se/](https://curl.haxx.se/)
* Used libopenssl for a working with https requests: [https://www.openssl.org/](https://www.openssl.org/)
* Used libzip for a working with zip-archives: [https://libzip.org/](https://libzip.org/)
* Used libpng for a processing png-images: [http://libpng.org/](http://libpng.org/)


