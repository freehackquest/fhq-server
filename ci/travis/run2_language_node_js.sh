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


# hardcode for build travis
cd web-user
check_ret $? "change directory to web-user"
npm -v
check_ret $? "npm -v"
node -v
check_ret $? "node -v"
npm install
check_ret $? "npm install requirements for web-user"
npm run build-prod
check_ret $? "angular build-prod web-user"
cd ..