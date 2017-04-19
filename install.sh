#!/bin/bash

ln -s `pwd`/etc/freehackquest-backend /etc/freehackquest-backend
ln -s `pwd`/etc/init.d/freehackquest-backend /etc/init.d/freehackquest-backend
ln -s `pwd`/freehackquestd /usr/bin/freehackquest-backend
ln -s `pwd`/var/log/freehackquest-backend /var/log/freehackquest-backend
update-rc.d -f freehackquest-backend remove
update-rc.d freehackquest-backend defaults
