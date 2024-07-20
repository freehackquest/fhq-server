# FHQ Server (backend + frontend)

[![Docker Pulls](https://img.shields.io/docker/pulls/sea5kg/fhq-server.svg)](https://hub.docker.com/r/sea5kg/fhq-server/) [![Github Stars](https://img.shields.io/github/stars/freehackquest/fhq-server.svg?label=github%20%E2%98%85)](https://github.com/freehackquest/fhq-server/) [![Github Stars](https://img.shields.io/github/contributors/freehackquest/fhq-server.svg)](https://github.com/freehackquest/fhq-server/) [![Github Forks](https://img.shields.io/github/forks/freehackquest/fhq-server.svg?label=github%20forks)](https://github.com/freehackquest/fhq-server/) [![Total alerts](https://img.shields.io/lgtm/alerts/g/freehackquest/fhq-server.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/freehackquest/fhq-server/alerts/) [![Language grade: Python](https://img.shields.io/lgtm/grade/python/g/freehackquest/fhq-server.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/freehackquest/fhq-server/context:python) [![Language grade: Python](https://img.shields.io/lgtm/grade/cpp/g/freehackquest/fhq-server.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/freehackquest/fhq-server/context:cpp) [![Language grade: JavaScript](https://img.shields.io/lgtm/grade/javascript/g/freehackquest/fhq-server.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/freehackquest/fhq-server/context:javascript) [![Server API Coverage](https://img.shields.io/badge/Server%20API%20Coverage-39%25-blue.svg)](https://travis-ci.com/freehackquest/fhq-server)

Backend && Frontend for FreeHackQuest on Qt and WebSockets

## Contacts

* [Group in VK](https://vk.com/freehackquest)
* [Group in Telegram](https://telegram.me/freehackquest)

## docker-compose example (easy start)

**>= v0.2.50**

Step 1. Create a file `docker-compose.yml` with content:
```
version: '3'

services:
  fhqserver_db:
    image: mysql:5.7
    command: --default-authentication-plugin=mysql_native_password
    volumes:
      - "./freehackquest_docker/mysql_data:/var/lib/mysql"
    restart: always
    environment:
      MYSQL_ROOT_PASSWORD: KzhyntJxwt
      MYSQL_DATABASE: fhqserver
      MYSQL_USER: fhqserver
      MYSQL_PASSWORD: VveGJemxFb
    networks:
      - fhqserver_net

  fhqserver:
    depends_on:
      - fhqserver_db
    image: sea5kg/fhq-server:latest
    volumes:
      - "./freehackquest_docker:/opt/fhq-server"
    environment:
      FREEHACKQUEST_DATABASE_HOST: fhqserver_db # host of database, so it's name of service
      FREEHACKQUEST_DATABASE_PORT: 3306 # mysql default port
      FREEHACKQUEST_DATABASE_NAME: fhqserver # -> MYSQL_DATABASE
      FREEHACKQUEST_DATABASE_USER: fhqserver # -> MYSQL_USER
      FREEHACKQUEST_DATABASE_PASS: VveGJemxFb # -> MYSQL_PASSWORD
    ports:
      - "1234:1234"
      - "4613:4613"
      - "7080:7080"
    restart: always
    networks:
      - fhqserver_net

  ext_nginx_node:
    depends_on:
      - fhqserver_db
      - fhqserver
    image: nginx:1.16
    volumes:
      - "./freehackquest_docker/nginx/html/:/usr/share/nginx/html"
      - "./freehackquest_docker/nginx/conf.d:/etc/nginx/conf.d"
      - "./freehackquest_docker/nginx/logs:/var/log/nginx/"
      - "./freehackquest_docker/ssl:/etc/nginx/ssl"
    restart: always
    ports:
      - "801:80"
      - "4431:443"
    networks:
      - fhqserver_net

networks:
  fhqserver_net:
    driver: bridge
```

Step 2. Start services

```
$ docker-compose up
```

Step 3. Open a urls 

User interface: http://localhost:801/

Admin interface: http://localhost:801/admin/

*Note: default login/password: admin/admin*

<!-- * [INSTALL_FROM_PPA](https://github.com/freehackquest/fhq-server/tree/master/install/INSTALL_FROM_PPA.md) 

* Configure autostart
    * [SYSTEMD](install/SYSTEMD.md)
    * [INITD](install/INITD.md)
-->

## Distribution

| What                    | Where         |  Source code |
| ----------------------- | ------------- | ------------ |
| fhq-server (docker)     | [![Docker Pulls](https://img.shields.io/docker/pulls/sea5kg/fhq-server.svg)](https://hub.docker.com/r/sea5kg/fhq-server/)  |  |
| fhq-server (vm)         | [https://freehackquest.com/dist/](https://freehackquest.com/dist/)  | |
| client library (js)     | [![npm](https://img.shields.io/npm/v/freehackquest-libclient-web-js)](https://www.npmjs.com/package/freehackquest-libclient-web-js) | [GitHub](https://github.com/freehackquest/freehackquest-libclient-web-js) |
| client library (python) | [![PyPI](https://img.shields.io/pypi/v/freehackquest-libclient-py)](https://pypi.org/project/freehackquest-libclient-py/) | [GitHub](https://github.com/freehackquest/freehackquest-libclient-py) |

## For developers

* [Doxygen Documentation](https://freehackquest.com/doxygen/)
* [trello](https://trello.com/b/Z7blDGj3/fhq-server)
* [Документация к системе](https://docs.google.com/document/d/16toRLzVUy8tkilW-jFVjG-1g-8ILDICfucO3EwK8oTg/edit?usp=sharing)

### Build all components (c++ && angular)

#### Ubuntu / Debian 11.0

Requirements:

```
$ sudo apt install git-core g++ make cmake qtchooser qt5-default \
   libqt5websockets5 libqt5websockets5-dev libqt5sql5-mysql \
   libwebsockets-dev libcurl4-openssl-dev \
   zlib1g zlib1g-dev \
   python3 python3-pip \
   libpng-dev \
   libmysqlclient-dev \
   mysql-server mysql-client \
   build-essential curl
```

*qt5-default - no exists on Ubuntu 24.04*

Install latest nodejs (for web-user)

```
$ curl -sL https://deb.nodesource.com/setup_16.x | sudo -E bash -
$ sudo apt-get install nodejs
```

Configure database:

Run mysql console:
```
$ mysql -h localhost -u root -p
```

Execute next queries for create empty database:
```
> CREATE DATABASE `fhqtravis` CHARACTER SET utf8 COLLATE utf8_general_ci;
> CREATE USER 'travis'@'localhost' IDENTIFIED BY '';
> GRANT ALL PRIVILEGES ON fhqtravis.* TO 'travis'@'localhost' WITH GRANT OPTION;
> FLUSH PRIVILEGES;
```

Clone repository:
```
$ git clone https://github.com/freehackquest/fhq-server ~/fhq-server.git
```

Build angular:
```
$ cd ~/fhq-server.git/web-user
$ npm install
$ npm run build --prod
```

Build c++ server:

```
$ cd ~/fhq-server.git/
$ ./build_simple.sh
$ ./fhq-server -wd ./ci/travis/data start
```

* web-user you can see here: `http://localhost:7080/`
* web-admin: `http://localhost:7080/admin/`

*Note: default login/password: admin/admin*


Also you can start web angular for develop:
*Will need backend runned (!)*
```
$ cd ~/fhq-server.git/web-user
$ npm run start
```
So you can see web user here: `http://localhost:4200/`

### Unit-tests 

Build and run unit-tests:

```
$ cd ~/fhq-server.git/unit-tests.wsjcpp
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
$ ./update_freehackquest_libclient_py.sh # update auto-generate-client-library
$ python3 run_tests.py # run tests
```

New running tests (based on pytest):

```
$ cd ~/fhq-server.git/tests/server-api-tests/
$ ./update_freehackquest_libclient_py.sh # update auto-generate-client-library
$ python3 -m pylint --rcfile=.pylintrc *.py
$ python3 -m pytest -rAs -c env-travis.ini .
```

### Server api tests - second way (docker mode)

Since you do not have a local server, you need to install the client library:

```
$ pip3 install freehackquest-libclient-py
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

### Check the leaks mememory via valgrind

Requirements:

```
$ sudo apt install valgrind # ubuntu and debian
```

Terminal1:
```
$ cd ~/fhq-server.git/
$ valgrind --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --verbose \
    --log-file=valgrind-out.txt \
    ./fhq-server -wd ./ci/travis/data
```

Terminal2:
``` 
$ cd ~/fhq-server.git/fhq-server-tests
$ python3 run_tests.py
```
or run new pytests
``` 
$ cd ~/fhq-server.git/tests/server-api-tests
$ python3 -m pytest -rAs -c env-local.ini .
```

### Web User Interface

Fast to fix and develop for current server version

```
$ git clone git@github.com:freehackquest/fhq-server.git ~/fhq-server.git
$ cd ~/fhq-server.git/web-user
$ npm install 
$ npm run start
```

In a file `~/fhq-server.git/web-user/src/app/services/fhq.service.ts`
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

## Build docker image

If you wanna fresh enviroment
```
$ ./rebuild_environment_images.sh
```

Rebuild fresh docker 
```
$ docker build -t sea5kg/fhq-server:latest .
$ docker tag sea5kg/fhq-server:latest sea5kg/fhq-server:v0.2.xx
```

## Ubuntu - install from ppa (outdated)

*Note: Not working now*

[https://launchpad.net/~freehackquest/+archive/ubuntu/fhq-server](https://launchpad.net/~freehackquest/+archive/ubuntu/fhq-server)

```
$ sudo add-apt-repository ppa:freehackquest/fhq-server
$ sudo apt update
$ sudo apt install fhq-server
```

## Configure systemd on host system (Ubuntu/Debian)

*Note: Before your need build of fhq-server and install to /usr/bin/fhq-server*
*Note: Prepare mysql empty database for fhq-server*
*Note: last updated for fhq-server-v0.2.35*

Create the file `/etc/systemd/system/fhq-server.service` with content

```
[Unit]
Description=FreeHackQuest Server
After=syslog.target
After=network.target
After=mysql.service
Requires=mysql.service

[Service]
WorkingDirectory=/usr/share/fhq-server
User=root
Group=root
ExecStart=/bin/sh -c '/usr/bin/fhq-server start > /var/log/fhq-server/access.log 2> /var/log/fhq-server/error.log'

TimeoutSec=30
Restart=always

[Install]
WantedBy=multi-user.target
Alias=fhq-server.service
```

Create directories

```
$ sudo mkdir /etc/fhq-server/
$ sudo mkdir /var/log/fhq-server/
$ sudo mkdir /usr/share/fhq-server/
$ sudo mkdir /usr/share/fhq-server/web-admin
$ sudo mkdir /usr/share/fhq-server/web-user
$ sudo mkdir /usr/share/fhq-server/fhqjad-store
```

Create config file `/etc/fhq-server/fhq-server.conf` with content
```
## Database Configuration
usemysql = yes
storage_type = mysql
dbhost = 127.0.0.1
dbname = fhq
dbport = 3306
dbuser = fhq
dbpass = fhq

## Server configurations
port = 1234
ssl_on = no
ssl_port = 4613
ssl_key_file = /etc/ssl/private/test-selfsigned.key
ssl_cert_file = /etc/ssl/certs/test-selfsigned.crt

# Web Configuration
web_port = 7080
web_max_threads = 1
web_admin_folder = /usr/share/fhq-server/web-admin
web_user_folder = /usr/share/fhq-server/web-user

web_public_folder = /usr/share/fhq-server/public/
web_public_folrer_url = http://localhost:7080/public/

# Jobs Pool Config
jobs_fast_threads = 2
jobs_slow_threads = 1

# fhqjad-store
web_fhqjad_store = /usr/share/fhq-server/fhqjad-store
```

Reload and restart
```
$ sudo systemctl daemon-reload
$ sudo systemctl enable fhq-server
$ sudo systemctl restart fhq-server.service
```

Uninstall

```
$ sudo systemctl stop fhq-server.service
$ sudo systemctl disable fhq-server.service
$ sudo rm /etc/systemd/system/fhq-server.service
$ sudo systemctl daemon-reload
$ sudo systemctl reset-failed
```

## Sample for init.d script and watchdog cron script

Create a file `/etc/init.d/fhq-server` with content:
```
#!/bin/bash

### BEGIN INIT INFO
# Provides:          fhq-server
# Short-Description: Start daemon at boot time
# Description:       Enable service provided by daemon.
# Required-Start:    $remote_fs $syslog $network mysql
# Required-Stop:     $remote_fs $syslog
# Should-Start:
# Should-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:
### END INIT INFO

DAEMON_PATH="/usr/bin/fhq-server"
NAME=freehackquestd
DESC="FreeHackQuest Server"

PIDFILE=/var/run/$NAME.pid
case "$1" in
start)
    printf "%-50s" "Starting $NAME..."
    DATE=`date +%Y%m%d-%H%M%S`
    PID=`/usr/bin/fhq-server -s > /var/log/fhq-server/access-$DATE.log 2> /var/log/fhq-server/error-$DATE.log & echo $!`
    # echo "Saving PID" $PID " to " $PIDFILE
    if [ -z $PID ]; then
        printf "%s\n" "Fail"
    else
        echo $PID > $PIDFILE
        printf "%s\n" "Ok"
    fi ;;
status)
    printf "%-50s" "Checking $NAME..."
    if [ -f $PIDFILE ]; then
        PID=`cat $PIDFILE`
        if [ -z "`ps axf | grep ${PID} | grep -v grep`" ]; then
            printf "%s\n" "Process dead but pidfile exists"
        else
            echo "Running"
        fi
    else
        printf "%s\n" "Service not running"
    fi
;;
stop)
    printf "%-50s" "Stopping $NAME"
    PID=`cat $PIDFILE`
    if [ -f $PIDFILE ]; then
        kill -HUP $PID
        printf "%s\n" "Ok"
        rm -f $PIDFILE
    else
        printf "%s\n" "pidfile not found"
    fi
;;
restart)
    $0 stop
    $0 start
;;
force-reload)
    $0 stop
    $0 start
;;
*)
    echo "Usage: $0 {status|start|stop|restart|force-reload}"
    exit 1
esac
```

Register and add to autostart

```
$ sudo update-rc.d -f fhq-server remove
$ sudo update-rc.d fhq-server defaults
$ sudo /etc/init.d/fhq-server start
```

or for start

```
$ service fhq-server start
```

**Watchdog Cron config**

Create a file `/opt/watchdog-fhq-server.sh` with content:

```
#!/bin/bash

COUNT=$(ps -aux | grep fhq-server | wc -l)

if [ $COUNT -ne 2 ]; then
	echo "Need restart backend"
	service fhq-server stop
	service fhq-server restart
fi
```

Run edit cron config:

```
$ sudo crontab -e
```

And add to cron line: `* * * * * /opt/watchdog-fhq-server.sh`


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

web-admin paginator:
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

## Publishing client libraries


### js library

generate new library
```
$ ./fhq-server export-libfhqcli-web-javascript
```
After this merge generated files from `freehackquest-libclient-web-js` to repository https://github.com/freehackquest/freehackquest-libclient-web-js


Publishing will be some like this:

```
$ git clone https://github.com/freehackquest/freehackquest-libclient-web-js
$ cd freehackquest-libclient-web-js
$ npm login 
Username: your_login
Password: (password hidden)
Email: (this IS public) your@mail
$ npm publish .
....
+ freehackquest-libclient-web-js@x.x.x
```

### python (pypi)

generate new library
```
$ ./fhq-server api export-freehackquest-libclient-py
```
After this merge generated files from `freehackquest-libclient-py` to repository https://github.com/freehackquest/freehackquest-libclient-py


```
$ git clone https://github.com/freehackquest/freehackquest-libclient-py
$ cd freehackquest-libclient-py
$ rm -rf build
$ rm -rf dist
$ rm -rf freehackquest_libclient_py.egg-info

$ python3 setup.py sdist bdist_wheel
$ python3 -m twine upload dist/*
```


## 3rdParty

* C++ Source Code Managment: [wsjcpp](https://wsjcpp.org)
* For Json: [nlohmann::json](https://github.com/nlohmann/json)
* Used libcurl for a working with http/https/smtp: [https://curl.haxx.se/](https://curl.haxx.se/)
* Used libopenssl for a working with https requests: [https://www.openssl.org/](https://www.openssl.org/)
* Used libzip for a working with zip-archives: [https://libzip.org/](https://libzip.org/)
* Used libpng for a processing png-images: [http://libpng.org/](http://libpng.org/)


## Thanks For

* [Charts.js](http://www.chartjs.org/docs/)
* Sergey Belov (found xss!)
* Igor Polyakov
* Maxim Samoilov (Nitive)
* Dmitrii Mukovkin
* Team Keva
* Alexey Gulyaev
* Alexander Menschikov
* Ilya Bokov
* Extrim Code
* Sergey Ushev
* Danil Dudkin
