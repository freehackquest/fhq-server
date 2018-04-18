#!/bin/bash


./clean_sources_ppa.sh

cd ../../
tar -acf fhq-server_0.2.4.orig.tar.gz fhq-server
cd fhq-server
debuild -S -sa
cd ../install/PPA
