#!/bin/bash

SOURCES=`pwd`/../../fhq-server

if [[ "$(docker images -q fhq-server:build-fhq-server 2> /dev/null)" == "" ]]; then
    docker build --rm=true -t "fhq-server:build-fhq-server" .
fi

if [ ! "$(docker ps -q -f name=build-fhq-server)" ]; then
    if [ "$(docker ps -aq -f status=exited -f name=build-fhq-server)" ]; then
        # cleanup
        docker rm build-fhq-server
    fi
    # echo $SOURCES " -> /root/sources"
    docker run -it --name=build-fhq-server -v "$SOURCES:/root/sources" fhq-server:build-fhq-server
fi