# FHQ Server (backend)

[![Build Status](https://travis-ci.org/freehackquest/fhq-server.svg?branch=master)](https://travis-ci.org/freehackquest/fhq-server)

Backend for FreeHackQuest on Qt and WebSockets
Frontend for FreeHackQuest here: [frontend](https://github.com/freehackquest/frontend)

* [CONFIGURING](install/CONFIGURING.md)
* [HOW-TO-BUILD](install/HOW-TO-BUILD.md)
* [PRE-BUILDS](install/PRE-BUILDS.md)
* [DOCKER-COMPOSE](https://github.com/freehackquest/fhq-server/tree/master/install/DOCKER_COMPOSE)
* Configure autostart
	* [SYSTEMD](install/SYSTEMD.md)
	* [INITD](install/INITD.md)

## Install from docker hub:

[image on hub.docker.com](https://hub.docker.com/r/freehackquest/fhq-server)

Example of docker-compose.yml here:

[docker-compose](https://github.com/freehackquest/fhq-server/tree/master/install/DOCKER_COMPOSE)


## (Ubuntu) Install from ppa:

```
$ sudo add-apt-repository ppa:freehackquest/fhq-server
$ sudo apt update
$ sudo apt install fhq-server
```
More details here:

[https://launchpad.net/~freehackquest/+archive/ubuntu/fhq-server](https://launchpad.net/~freehackquest/+archive/ubuntu/fhq-server)

## Client libraries

* [npm javascript](https://www.npmjs.com/package/libfhqcli-web-js)
* [python](https://pypi.org/project/libfhqcli/)

## 3rdParty

* For Json: [nlohmann::json](https://github.com/nlohmann/json)
* Used libcurl for a working with http/https/smtp: [https://curl.haxx.se/](https://curl.haxx.se/)
* Used libopenssl for a working with https requests: [https://www.openssl.org/](https://www.openssl.org/)
* Used libzip for a working with zip-archives: [https://libzip.org/](https://libzip.org/)
* Used libpng for a processing png-images: [http://libpng.org/](http://libpng.org/)
