# systemd

* [CONFIGURING](install/CONFIGURING.md)
* [HOW-TO-BUILD](install/HOW-TO-BUILD.md)


## Copy init.d script:

```
$ sudo cp ./INITD/fhq-server  /etc/init.d/fhq-server
````

## Register and add to autostart
```
$ sudo update-rc.d -f fhq-server remove
$ sudo update-rc.d fhq-server defaults
$ sudo /etc/init.d/fhq-server start
```

Or you can use:
```
$ service fhq-server start
```

## Check running service by cron

```
$ sudo cp ./INITD/check-backend.sh  /usr/share/fhq-server/check-backend.sh
````

Run edit cron config:

```
$ sudo crontab -e
```

And add to cron line: `* * * * * cd /usr/share/fhq-server/ && ./check-backend.sh`


