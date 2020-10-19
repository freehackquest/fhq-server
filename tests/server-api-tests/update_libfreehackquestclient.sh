#!/bin/bash

cd ../../fhq-server
rm -rf ./libfreehackquest-client-py
./fhq-server export-libfreehackquest-client-py
cd ../tests/server-api-tests
rm -rf libfreehackquestclient
cp -r ../../fhq-server/libfreehackquest-client-py/libfreehackquestclient libfreehackquestclient
rm -rf ../../fhq-server/libfreehackquest-client-py