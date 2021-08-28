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

TAG=v$(date +"%Y-%m-%d")

docker build --rm --tag sea5kg/fhq-server-build-environment:$TAG -f Dockerfile.build-environment .
check_ret $? "Could not build fhq-server-build-environment"
docker build --rm --tag sea5kg/fhq-server-build-environment:latest -f Dockerfile.build-environment .
docker build --rm --tag sea5kg/fhq-server-release-environment:$TAG -f Dockerfile.release-environment .
check_ret $? "Could not build fhq-server-release-environment"
docker build --rm --tag sea5kg/fhq-server-release-environment:latest -f Dockerfile.release-environment .
