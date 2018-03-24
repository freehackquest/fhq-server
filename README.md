# FreeHackQuest Backend

Backend for FreeHackQuest on Qt and WebSockets
Frontend for FreeHackQuest here: [https://github.com/freehackquest/frontend](https://github.com/freehackquest/frontend)

## Requirements

Mysql database modules:
```
$ sudo apt-get install mysql-server
$ sudo apt-get install mysql-client
```
Create database by sql-queries:

```
> CREATE DATABASE `freehackquest` CHARACTER SET utf8 COLLATE utf8_general_ci;
> CREATE USER 'freehackquest_u'@'localhost' IDENTIFIED BY 'freehackquest_password_database';
> GRANT ALL PRIVILEGES ON freehackquest.* TO 'freehackquest_u'@'localhost' WITH GRANT OPTION;
> FLUSH PRIVILEGES;
```
## Pre-builds

Prebuild packages: http://dist.freehackquest.com/backend/
```
$ sudo apt update
$ sudo apt install qt5-default libqt5websockets5 libqt5sql5-mysql
$ wget "http://dist.freehackquest.com/backend/amd64/debian/8.0 (jessie)/freehackquest-backend_0.1.15_amd64.deb"
$ su
# apt install mysql-server
# dpkg -i freehackquest-backend_0.1.15_amd64.deb
# apt -f install
# cp /etc/freehackquest-backend/conf.ini.example /etc/freehackquest-backend/conf.ini
# nano /etc/freehackquest-backend/conf.ini
```
Run for check database configuration:
```
# freehackquest-backend
```
Run like service (or just reboot system):
```
# /etc/init.d/freehackquestd start
```
## Build

### Requirements
```
$ sudo apt install g++ make qtchooser qt5-default libqt5websockets5 libqt5websockets5-dev libqt5sql5-mysql
```
### Build
```
$ qmake
$ make
```
### Install to usr/bin
```
$ sudo ln -s `pwd`/freehackquest-backend /usr/bin/freehackquest-backend
```
### Install directory for logs
```
$ sudo ln -s `pwd`/var/log/freehackquest-backend /var/log/freehackquest-backend
```
### Install and edit configuration:
```
$ cp ./etc/freehackquest-backend/conf.ini.example ./etc/freehackquest-backend/conf.ini
$ sudo ln -s `pwd`/etc/freehackquest-backend /etc/freehackquest-backend
$ sudo nano /etc/freehackquest-backend/conf.ini
```
### Configure autostart binary
```
$ sudo ln -s `pwd`/etc/init.d/freehackquest-backend /etc/init.d/freehackquest-backend
$ sudo update-rc.d -f freehackquest-backend remove
$ sudo update-rc.d freehackquest-backend defaults
$ sudo /etc/init.d/freehackquest-backend start
```
Or you can use:
```
$ service freehackquest-backend start

## 3rdParty

* Contains source code to compile: [SmtpClient-for-Qt](https://github.com/bluetiger9/SmtpClient-for-Qt)
* For Json: [Json](https://github.com/nlohmann/json)
