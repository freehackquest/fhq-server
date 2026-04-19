#!/bin/bash
##################################################################################
#                      Project
#    __ _
#   / _| |__   __ _       ___  ___ _ ____   _____ _ __
#  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
#  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
#  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
#                |_|
#
# MIT License
#
# Copyright (c) 2011-2026 FreeHackQuest <freehackquest@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Original repository: https://github.com/freehackquest/fhq-server
#
##################################################################################

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

./build_simple.sh
check_ret $? "build fhq-server"
./fhq-server -wd ./ci/travis/data version
check_ret $? "print version of fhq-server"
./fhq-server -wd ./ci/travis/data db test
check_ret $? "test connection to database"

mkdir -p "./web-user/dist/"

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
