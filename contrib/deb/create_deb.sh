#!/bin/bash
##################################################################################
#                      Project
#    __ _
#   / _| |__   __ _       ___  ___ _ ____   _____ _ __
#  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
#  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
#  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
#                |_|
#
# MIT License
#
# Copyright (c) 2011-2026 FreeHackQuest <freehackquest@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Original repository: https://github.com/freehackquest/fhq-server
#
##################################################################################

CURRENT_FOLDER=$(pwd)
echo $CURRENT_FOLDER;

FHQ_SERVER_FOLDER=$CURRENT_FOLDER/../../fhq-server

name="fhq-server"

# remove old deb package
find ./ -name *.deb  | while read f; do  rm "$f"; done

# clear old lintian log
rm -f "lintian.log"


# build latest version of binary
cd $FHQ_SERVER_FOLDER
qmake && make
cd $CURRENT_FOLDER

# prepare tmp folder
rm -rf tmpdeb
mkdir tmpdeb
cp -R "DEBIAN" "tmpdeb/"
mkdir -p "tmpdeb/usr/share/doc"
mkdir -p "tmpdeb/usr/share/doc/$name"
mkdir -p "tmpdeb/usr/share/$name/libs"
mkdir -p "tmpdeb/etc/init.d"
mkdir -p "tmpdeb/var/log/$name"
mkdir -p "tmpdeb/etc/$name"
mkdir -p "tmpdeb/usr/share/doc/$name"

cp -R "../../LICENSE" "tmpdeb/usr/share/doc/$name/copyright"

mkdir "tmpdeb/usr/bin"
cp $FHQ_SERVER_FOLDER/$name "tmpdeb/usr/bin/$name"
strip -R -o "tmpdeb/usr/bin/$name" "tmpdeb/usr/bin/$name"
cp -R ../INITD/fhq-server tmpdeb/etc/init.d/
cp -R ../CONFIGURING/conf.ini.example tmpdeb/etc/$name/
cp -R ../CONFIGURING/conf.ini.example tmpdeb/etc/$name/
cp -R ./changelog tmpdeb/usr/share/doc/$name/changelog

cd tmpdeb

find -type f | grep -re ~$ | while read f; do rm -rf "$f"; done

gzip -9 usr/share/doc/$name/changelog

# todo manual
# gzip -9 "usr/share/man/man1/fhq-server.1"

# help: https://www.debian.org/doc/manuals/maint-guide/dreq.ru.html

if [ ! -d "DEBIAN" ]; then
	mkdir "DEBIAN"  
fi

# config files
echo "/etc/fhq-server/conf.ini.example" >> DEBIAN/conffiles
echo "/etc/init.d/fhq-server" >> DEBIAN/conffiles

# create md5sums
echo -n "" > DEBIAN/md5sums
find "." -type f | while read f; do
	md5sum "$f" >> DEBIAN/md5sums
done

find usr -type f | while read f; do  chmod 644 "$f"; done
find etc -type f | while read f; do  chmod 644 "$f"; done
find var -type f | while read f; do  chmod 644 "$f"; done
find DEBIAN -type f | while read f; do  chmod 644 "$f"; done

find usr -type d | while read d; do  chmod 755 "$d"; done
find etc -type d | while read d; do  chmod 755 "$d"; done
find var -type d | while read d; do  chmod 755 "$d"; done
find DEBIAN -type d | while read d; do  chmod 755 "$d"; done

chmod +x etc/init.d/fhq-server
chmod +x usr/bin/fhq-server
chmod +x DEBIAN/preinst
chmod +x DEBIAN/postinst
chmod +x DEBIAN/prerm
chmod +x DEBIAN/postrm

find usr -type f | while read f; do md5sum "$f"; done > DEBIAN/md5sums
find etc -type f | while read f; do md5sum "$f"; done >> DEBIAN/md5sums
find var -type f | while read f; do md5sum "$f"; done >> DEBIAN/md5sums

cd ..

echo "from deb-pkg_create"

#build
fakeroot dpkg-deb --build tmpdeb ./

# todo uncommneted:
# rm -rf tmpdeb

#check
lintian *.deb > lintian.log
cat lintian.log
