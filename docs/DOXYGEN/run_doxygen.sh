#!/bin/bash

FHQSRV_VERSION=$(cat ../../fhq-server/CMakeLists.txt | grep -Po '(?<=FHQSRV_VERSION=")\d+\.\d+\.\d+')
echo $FHQSRV_VERSION

export FHQSRV_VERSION="$FHQSRV_VERSION" 

doxygen
