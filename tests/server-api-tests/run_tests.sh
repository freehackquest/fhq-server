#!/bin/bash
# Copyright (C) 2011-2022, Evgenii Sopov <mrseakg@gmail.com>

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

python3 -m pylint --rcfile=.pylintrc *.py
check_ret $? "pylint"

python3 -m pytest -rAs -c env-travis.ini
check_ret $? "pytest"

# pytest-3 -rAs -c env-docker.ini tests_leaks.py