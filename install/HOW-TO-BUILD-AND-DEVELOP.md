# HOW TO BUILD AND DEVELOP

## Requirements

Ubuntu / Debian 10.0

### Install git

```
$ sudo apt install git
```


### Install database

```
$ sudo apt install mysql-server mysql-client
```

### Install c++ requirements

```
$ sudo apt install g++ make qtchooser qt5-default libqt5websockets5 libqt5websockets5-dev libqt5sql5-mysql
$ sudo apt install libwebsockets-dev libcurl4-openssl-dev
$ sudo apt install zlibc zlib1g zlib1g-dev
$ sudo apt install libpng-dev
$ sudo apt install libmysqlclient-dev
```

### Install latest nodejs

```
$ sudo apt-get install curl
$ curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -
```

## Building

### Download source code

Prepare folder:

```
$ mkdir -p ~/freehackquest/
$ cd ~/freehackquest/
$ git clone https://github.com/freehackquest/fhq-server fhq-server.git
```

### Configuring database (test)

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

### Build c++ backend

```
$ cd ~/freehackquest/fhq-server.git/fhq-server
$ ./build_simple.sh
```

### Build frontend

Will be build web site base on angular 8

```
$ cd ~/freehackquest/fhq-server.git/fhq-web-user
$ npm run build-prod
```

## Run server

Run server:
```
$ cd ~/freehackquest/fhq-server.git/fhq-server
$ ./fhq-server -wd ../data_travis start
```

* Web you can see here: `http://localhost:7080/`
* Administrative panel: `http://localhost:7080/admin/`

default login/password: admin/admin


## Run web develop:

Will need backend runned (!)

```
$ cd ~/freehackquest/fhq-server.git/fhq-web-user
$ npm run start
```

So you can see web user here: `http://localhost:1234/`