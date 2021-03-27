#!/bin/bash

check_ret() {
    if [ $1 -ne 0 ]; then
        echo ""
        echo "!!! FAIL: $3"
        echo "********************************************************************************"
        echo ""
        exit $1
    else
        echo ""
        echo "*** SUCCESS: $2"
        echo "********************************************************************************"
        echo ""
    fi
} 

cd ../
check_ret $? "Change to .."
rm -rf ./libfreehackquest-client-py
./fhq-server export-libfreehackquest-client-py
check_ret $? "./fhq-server export-libfreehackquest-client-py"
cd ./fhq-server-tests
check_ret $? "cd ./fhq-server-tests"
rm -rf libfreehackquestclient
cp -r ../libfreehackquest-client-py/libfreehackquestclient libfreehackquestclient
check_ret $? "Copy"
rm -rf ../libfreehackquest-client-py