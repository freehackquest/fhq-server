#!/bin/bash

cd ../fhq-server
./fhq-server export-cli-library-python
cd ../tests
rm -rf libfhqcli
cp -r ../fhq-server/libfhqcli-py/libfhqcli libfhqcli