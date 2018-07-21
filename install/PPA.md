
# Uploading to ppa

Base information got from here: http://simmesimme.github.io/lessons/2011/11/02/ppa-launchpad-cmake

## Install requirements

	`sudo apt-get install build-essential cmake devscripts`
	
## Build

	
	$ cd fhq-server
	$ mkdir build && cd build
	$ cmake ..
	$ make
	$ cd .. && ./fhq-server

## Cleanup

	$ rm -rf fhq-server/build
	$ rm fhq-server/fhq-server
	
## 

	$ cd fhq-server
	$ mkdir debian && cd debian


## ChangeLog

Here fhq-server/debian/changelog

For generate new date:

	$ date -R

## First upload

	$ tar -acf fhq-server_0.1.orig.tar.gz fhq-server
	$ cd fhq-server
	$ debuild -S -sa
	$ cd ..
	$ dput ppa:freehackquest/fhq-server fhq-server_0.1-0ppa0_source.changes

## next time uploading

	$ cd fhq-server
	$ debuild -S -sd
	$ cd ..
	$ dput ppa:freehackquest/fhq-server fhq-server_0.1-0ppa0_source.changes
