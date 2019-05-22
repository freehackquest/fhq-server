#!/bin/bash

check_ret() {
    if [ $1 -ne 0 ]; then
        echo "$3"
        exit $1;
    else
        echo "$2"
    fi
} 

cd code-check
./build_simple.sh
check_ret $? "Success builded code-check" "Failed build"
cd ..
code-check/code-check --show-only-errors .
check_ret $? "Success passed code-check" "Failed"

cd unit-tests
./build_simple.sh
check_ret $? "Success builded unit-tests" "Failed build"
./unit-tests
check_ret $? "Success passed unit-tests" "Failed unit-tests"