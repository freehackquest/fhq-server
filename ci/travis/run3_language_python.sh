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

check_ret $? "change directory to fhq-server"
python3 -m sea5kg_cpplint
check_ret $? "sea5kg_cpplint"

./build_simple.sh
check_ret $? "build fhq-server"
./fhq-server -wd ./ci/travis/data version
check_ret $? "print version of fhq-server"
./fhq-server -wd ./ci/travis/data db test
check_ret $? "test connection to database"

mkdir -p "./web-user/dist/"


cd fhq-server-tests
check_ret $? "Change directory to tests"
./update_freehackquest_libclient_py.sh
check_ret $? "Update freehackquest_libclient_py python library"
python3 run_tests.py # run tests
check_ret $? "tests"
cd  ..

cd tests/server-api-tests
check_ret $? "Change directory to tests/server-api-tests"
./update_freehackquest_libclient_py.sh
check_ret $? "Update freehackquest_libclient_py python library"
python3 -m pylint --version
# in next time replace to 'pylint **/*.py'
python3 -m pylint --rcfile=.pylintrc *.py
check_ret $? "Check by pylint tests_leaks.py"
python3 -m pytest -rAs -c env-travis.ini
check_ret $? "Run tests_leaks.py"
cd ../..
