#!/bin/bash
# Copyright (C) 2011-2025, Evgenii Sopov <mrseakg@gmail.com>

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

cd ../..
check_ret $? "Change to ../.."
rm -rf ./freehackquest-libclient-py
./fhq-server api export-freehackquest-libclient-py
check_ret $? "./fhq-server api export-freehackquest-libclient-py"
cd ./tests/server-api-tests
check_ret $? "cd ./tests/server-api-tests"
rm -rf freehackquest_libclient_py
cp -r ../../freehackquest-libclient-py/freehackquest_libclient_py freehackquest_libclient_py
check_ret $? "Copy"
rm -rf ../../freehackquest-libclient-py