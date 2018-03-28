#!/bin/bash


./clean_sources_ppa.sh

tar -acf fhq-server_0.2.orig.tar.gz ../fhq-server
cd ../fhq-server
debuild -S -sa
cd ../ppa
