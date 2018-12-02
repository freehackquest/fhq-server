#!/bin/bash

mkdir -p fhq-config

docker run --name=fhq-server \
    -p 1234:1234 \
    -p 4613:4613 \
    -v `pwd`/fhq-config:/usr/share/fhq-server freehackquest/fhq-server:0.2.17