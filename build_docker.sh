#!/bin/bash

if [ ! -f "./fhq-server/fhq-server.debian95" ]; then
    cd misc/build_for_docker
    ./build_for_docker.sh
    cd ../../
fi

docker rmi "freehackquest/fhq-server:0.2.17"
docker build --rm=true -t "freehackquest/fhq-server:0.2.17" .