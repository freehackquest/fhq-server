#!/bin/bash

check_ret() {
    if [ $1 -ne 0 ]; then
        echo "FAIL: $3"
        exit $1
    else
        echo "SUCCESS: $2"
    fi
} 

cd code-check
check_ret $? "change directory to code-check"
./build_simple.sh
check_ret $? "build code-check"
./code-check --show-only-errors ..
check_ret $? "code-check"
cd ..

cd fhq-server/unit-tests
check_ret $? "change directory to unit-tests"
./build_simple.sh
check_ret $? "build unit-tests"
./unit-tests
check_ret $? "unit-tests"
cd ../..

cd fhq-web-main
check_ret $? "change directory to fhq-web-main"
npm install
check_ret $? "install requirements for fhq-web-main"
npm run build
check_ret $? "build fhq-web-main"
cd ..

cd fhq-server
check_ret $? "change directory to fhq-server"
qmake --version # TODO when will be removed all then qt need remove this line
./build_simple.sh
check_ret $? "build fhq-server"
./fhq-server -wd ../data_travis version
check_ret $? "print version of fhq-server"
./fhq-server -wd ../data_travis -cdc
check_ret $? "test connection to database"
mkdir -p public/games
check_ret $? "created public/games"
./fhq-server -wd ../data_travis -set "server_folder_public=`pwd`/public/"
check_ret $? "set server_folder"
cd ..

cd tests
check_ret $? "Change directory to tests"
./update_libfhqcli.sh
check_ret $? "update libfhqcli-python"
python run_tests.py # run tests
check_ret $? "tests"

