#!/bin/bash

COUNT=$(ps -aux | grep freehackquest-backend | wc -l)

if [ $COUNT -ne 2 ]; then
	echo "Need restart backend"
	service freehackquest-backend stop
	service freehackquest-backend restart
fi
