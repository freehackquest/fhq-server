# FHQ Server (backend)

[![Build Status](https://travis-ci.org/freehackquest/fhq-server.svg?branch=master)](https://travis-ci.org/freehackquest/fhq-server) [![Docker Pulls](https://img.shields.io/docker/pulls/freehackquest/fhq-server.svg)](https://hub.docker.com/r/freehackquest/fhq-server/) [![Docker layers](https://images.microbadger.com/badges/image/freehackquest/fhq-server.svg)](https://microbadger.com/images/freehackquest/fhq-server) [![Github Stars](https://img.shields.io/github/stars/freehackquest/fhq-server.svg?label=github%20%E2%98%85)](https://github.com/freehackquest/fhq-server/) [![Github Stars](https://img.shields.io/github/contributors/freehackquest/fhq-server.svg)](https://github.com/freehackquest/fhq-server/) [![Github Forks](https://img.shields.io/github/forks/freehackquest/fhq-server.svg?label=github%20forks)](https://github.com/freehackquest/fhq-server/)


Backend && Frontend for FreeHackQuest on Qt and WebSockets

**!!! FAST START YOUR GAME** [here (based on docker-compose)](https://github.com/freehackquest/freehackquest-start-game)!!!

* [HOW-TO-BUILD-AND-DEVELOP](https://github.com/freehackquest/fhq-server/tree/master/install/HOW-TO-BUILD-AND-DEVELOP.md)
* [PRE-BUILDS](https://github.com/freehackquest/fhq-server/tree/master/install/PRE-BUILDS.md)
* [INSTALL_FROM_PPA](https://github.com/freehackquest/fhq-server/tree/master/install/INSTALL_FROM_PPA.md)
* [DOXYGEN](https://freehackquest.com/doxygen/)
* Configure autostart
	* [SYSTEMD](install/SYSTEMD.md)
	* [INITD](install/INITD.md)

## Distribution

| What           | Where         |
| -------------- | ------------- |
| fhq-server     | [ubuntu/ppa](https://launchpad.net/~freehackquest/+archive/ubuntu/fhq-server)  |
| fhq-server     | [![Docker Pulls](https://img.shields.io/docker/pulls/freehackquest/fhq-server.svg)](https://hub.docker.com/r/freehackquest/fhq-server/)  |
| client library | [![npm version](https://badge.fury.io/js/freehackquest-libclient-js.svg)](https://badge.fury.io/js/freehackquest-libclient-web-js) |
| client library | [![PyPI version](https://badge.fury.io/py/libfreehackquestclient.svg)](https://badge.fury.io/py/libfreehackquestclient) |
| frontend       | [github/web](https://github.com/freehackquest/frontend) |

## 3rdParty

* For Json: [nlohmann::json](https://github.com/nlohmann/json)
* Used libcurl for a working with http/https/smtp: [https://curl.haxx.se/](https://curl.haxx.se/)
* Used libopenssl for a working with https requests: [https://www.openssl.org/](https://www.openssl.org/)
* Used libzip for a working with zip-archives: [https://libzip.org/](https://libzip.org/)
* Used libpng for a processing png-images: [http://libpng.org/](http://libpng.org/)
