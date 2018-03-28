# PRE-BUILDS

Prebuild packages: http://dist.freehackquest.com/backend/
```
$ sudo apt update
$ sudo apt install qt5-default libqt5websockets5 libqt5sql5-mysql libcurl4-openssl-dev
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


