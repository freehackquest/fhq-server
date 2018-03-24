# systemd

## Make and Install

```
$ cd ..
$ qmake && make
$ sudo make install
```

## copy (or replace) fhq-server.service

```
$ sudo cp ./SYSTEMD/fhq-server.service  /etc/systemd/system/fhq-server.service
```

## Create directories

```
$ sudo mkdir /var/log/fhq-server
$ sudo mkdir /usr/share/fhq-server
```

## Copy config

```
$ sudo cp ./etc/freehackquest-backend/conf.ini.example ./etc/freehackquest-backend/conf.ini
```

## Reload and restart

```
$ sudo systemctl daemon-reload
$ sudo systemctl enable fhq-server
$ sudo systemctl restart fhq-server.service
```

## Uninstall

```
$ sudo systemctl stop fhq-server.service
$ sudo systemctl disable fhq-server.service
$ sudo rm /etc/systemd/system/fhq-server.service
$ sudo systemctl daemon-reload
$ sudo systemctl reset-failed
```
