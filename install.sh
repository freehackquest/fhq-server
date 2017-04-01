#!/bin/bash

ln -s `pwd`/etc/freehackquestd /etc/freehackquestd
ln -s `pwd`/etc/init.d/freehackquestd /etc/init.d/freehackquestd
ln -s `pwd`/freehackquestd /usr/bin/freehackquestd
ln -s `pwd`/var/log/freehackquestd /var/log/freehackquestd
update-rc.d -f freehackquestd remove
update-rc.d freehackquestd defaults
