#!/bin/bash

rm /etc/freehackquest-backend
rm /etc/init.d/freehackquest-backend
rm /usr/bin/freehackquest-backend
rm /var/log/freehackquest-backend
update-rc.d -f freehackquest-backend remove
