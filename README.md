# FHQ Server (backend)

[![Build Status](https://travis-ci.org/freehackquest/fhq-server.svg?branch=master)](https://travis-ci.org/freehackquest/fhq-server) [![Docker Pulls](https://img.shields.io/docker/pulls/freehackquest/fhq-server.svg)](https://hub.docker.com/r/freehackquest/fhq-server/) [![Docker layers](https://images.microbadger.com/badges/image/freehackquest/fhq-server.svg)](https://microbadger.com/images/freehackquest/fhq-server) [![Github Stars](https://img.shields.io/github/stars/freehackquest/fhq-server.svg?label=github%20%E2%98%85)](https://github.com/freehackquest/fhq-server/) [![Github Stars](https://img.shields.io/github/contributors/freehackquest/fhq-server.svg)](https://github.com/freehackquest/fhq-server/) [![Github Forks](https://img.shields.io/github/forks/freehackquest/fhq-server.svg?label=github%20forks)](https://github.com/freehackquest/fhq-server/)


Backend for FreeHackQuest on Qt and WebSockets
Frontend for FreeHackQuest here: [frontend](https://github.com/freehackquest/frontend)

* [CONFIGURING](https://github.com/freehackquest/fhq-server/tree/master/install/install/CONFIGURING.md)
* [HOW-TO-BUILD](https://github.com/freehackquest/fhq-server/tree/master/install/install/HOW-TO-BUILD.md)
* [PRE-BUILDS](https://github.com/freehackquest/fhq-server/tree/master/install/install/PRE-BUILDS.md)
* [DOCKER-COMPOSE](https://github.com/freehackquest/fhq-server/tree/master/install/DOCKER_COMPOSE)
* [INSTALL_FROM_PPA](https://github.com/freehackquest/fhq-server/tree/master/install/INSTALL_FROM_PPA.md)
* Configure autostart
	* [SYSTEMD](install/SYSTEMD.md)
	* [INITD](install/INITD.md)

## Distribution

| What           | Where         |
| -------------- | ------------- |
| fhq-server     | [ubuntu/ppa](https://launchpad.net/~freehackquest/+archive/ubuntu/fhq-server)  |
| fhq-server     | [docker image](https://hub.docker.com/r/freehackquest/fhq-server)  |
| client library | [npm/javascript](https://www.npmjs.com/package/libfhqcli-web-js) |
| client library | [python](https://pypi.org/project/libfhqcli/) |
| frontend       | [github/web](https://github.com/freehackquest/frontend) |


## 3rdParty

* For Json: [nlohmann::json](https://github.com/nlohmann/json)
* Used libcurl for a working with http/https/smtp: [https://curl.haxx.se/](https://curl.haxx.se/)
* Used libopenssl for a working with https requests: [https://www.openssl.org/](https://www.openssl.org/)
* Used libzip for a working with zip-archives: [https://libzip.org/](https://libzip.org/)
* Used libpng for a processing png-images: [http://libpng.org/](http://libpng.org/)
