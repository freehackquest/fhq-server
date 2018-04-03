#!/bin/bash

COUNT=$(ps -aux | grep fhq-server | wc -l)

if [ $COUNT -ne 2 ]; then
	echo "Need restart backend"
	service fhq-server stop
	service fhq-server restart
fi
