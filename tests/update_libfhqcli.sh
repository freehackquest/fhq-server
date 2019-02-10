#!/bin/bash

cd ../fhq-server
./fhq-server export-libfhqcli-py
cd ../tests
rm -rf libfhqcli
cp -r ../fhq-server/libfhqcli-py/libfhqcli libfhqcli