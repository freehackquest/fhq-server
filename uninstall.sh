#!/bin/bash

rm /etc/freehackquestd
rm /etc/init.d/freehackquestd
rm /usr/bin/freehackquestd
rm /var/log/freehackquestd
update-rc.d -f freehackquestd remove
