#!/bin/bash

APP_VERSION=$(cat ../../fhq-server/src.wsjcpp/CMakeLists.txt | grep -Po '(?<=WSJCPP_APP_VERSION=")v\d+\.\d+\.\d+')
echo "Version=$APP_VERSION"

rm doxygen_objdb_*.tmp
rm doxygen_entrydb_*.tmp

export APP_VERSION="$APP_VERSION" 

doxygen
