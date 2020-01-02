#!/bin/bash

FHQSRV_VERSION=$(cat ../../fhq-server/CMakeLists.txt | grep -Po '(?<=FHQSRV_VERSION=")\d+\.\d+\.\d+')
echo $FHQSRV_VERSION

rm doxygen_objdb_*.tmp
rm doxygen_entrydb_*.tmp

export FHQSRV_VERSION="$FHQSRV_VERSION" 

doxygen
