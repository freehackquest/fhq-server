#!/bin/bash

mkdir -p dckr_data

docker run --name=fhq-server \
    -p 1234:1234 \
    -p 4613:4613 \
    -p 7080:7080 \
    -v `pwd`/dckr_fhqserver/data/public:/usr/share/fhq-server/public \
    freehackquest/fhq-server:0.2.17
