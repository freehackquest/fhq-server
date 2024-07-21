#!/bin/bash

check_ret() {
    if [ $1 -ne 0 ]; then
        echo ""
        echo "!!! FAIL: $2"
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

./pm.py code-check
check_ret $? "code-check"

cd unit-tests.wsjcpp
check_ret $? "change directory to unit-tests.wsjcpp"
./build_simple.sh
check_ret $? "build unit-tests"
./unit-tests
check_ret $? "unit-tests"
cd ..

qmake --version # TODO when will be removed all then qt need remove this line
./build_simple.sh
check_ret $? "build fhq-server"
./fhq-server -wd ./ci/travis/data version
check_ret $? "print version of fhq-server"
./fhq-server -wd ./ci/travis/data db test
check_ret $? "test connection to database"
