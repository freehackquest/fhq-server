#!/bin/bash

cd ../fhq-server
./fhq-server export-libfhqcli-py
cd ../fhq-server-tests
rm -rf libfhqcli
cp -r ../fhq-server/libfhqcli-py/libfhqcli libfhqcli
rm -rf ../fhq-server/libfhqcli-py