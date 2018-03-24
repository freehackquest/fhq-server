#!/bin/bash


rm fhq-server/debian/files
rm -rf fhq-server_0.2*
tar -acf fhq-server_0.2.orig.tar.gz fhq-server
cd fhq-server
debuild -S -sa
cd ..

dput ppa:freehackquest/fhq-server fhq-server_0.2-0ppa0_source.changes
