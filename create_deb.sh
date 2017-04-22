#!/bin/bash

version="0.1.`git rev-list HEAD --count`"
name="freehackquest-backend"

echo $version > version

# remove old deb package
find ./ -name *.deb  | while read f; do  rm "$f"; done

# clear old lintian log
echo "" > "lintian.log"
        
# build latest version of binary
qmake && make

# prepare tmp folder
rm -rf tmpdeb
mkdir tmpdeb
cp -R "DEBIAN" "tmpdeb/"
mkdir -p "tmpdeb/usr/share/doc"
mkdir -p "tmpdeb/usr/share/doc/freehackquest-backend"
mkdir -p "tmpdeb/usr/share/freehackquest-backend/libs"
cp -R "LICENSE" "tmpdeb/usr/share/doc/freehackquest-backend/copyright"

mkdir "tmpdeb/usr/bin"
cp "freehackquest-backend" "tmpdeb/usr/bin/freehackquest-backend"
cp -R etc tmpdeb/
cp -R var tmpdeb/
rm -rf "tmpdeb/etc/freehackquestd/conf.ini"

cd tmpdeb

find -type f | grep -re ~$ | while read f; do rm -rf "$f"; done

if [ ! -d "usr" ]; then
	mkdir "usr"
fi

if [ ! -d "usr/bin" ]; then
	mkdir "usr/bin"
fi

if [ ! -d "usr/share" ]; then
	mkdir "usr/share"  
fi


# find usr/share/bottlefs/ -name *~  | while read f; do  rm "$f"; done
# find usr/share/bottlefs/ -name .gitignore  | while read f; do  rm "$f"; done

# change log
echo "$name ($version) unstable; urgency=low" > usr/share/doc/freehackquest-backend/changelog.Debian
echo "" >> usr/share/doc/freehackquest-backend/changelog.Debian

git log --oneline | while read line
do
	echo "  * $line " >> usr/share/doc/freehackquest-backend/changelog.Debian
done
echo "" >> usr/share/doc/freehackquest-backend/changelog.Debian
echo " -- Evgenii Sopov <mrseakg@gmail.com> `date --rfc-2822` " >> usr/share/doc/freehackquest-backend/changelog.Debian
echo "" >> usr/share/doc/freehackquest-backend/changelog.Debian

gzip -9 usr/share/doc/freehackquest-backend/changelog.Debian

# todo manual
# gzip -9 "usr/share/man/man1/freehackquest-backend.1"

# help: https://www.debian.org/doc/manuals/maint-guide/dreq.ru.html

if [ ! -d "DEBIAN" ]; then
	mkdir "DEBIAN"  
fi

# config files
echo "etc/freehackquest-backend/conf.ini.example" >> DEBIAN/conffiles

# control
# todo section ???
# `arch`

size=($(du -s ./))
size=${size[0]}
echo "Source: $name
Section: misc
Priority: optional
Maintainer: Evgenii Sopov <mrseakg@gmail.com>
Depends: mysql-server libqt5websockets5 libqt5network5 libqt5sql5 libqt5core5a libc6 libstdc++6 libgcc1 zlib1g libicu52 libglib2.0-0 libpcre3
Version: $version
Installed-Size: $size
Homepage: https://github.com/freehackquest/backend
Package: $name
Architecture: amd64
Description: Backend for FreeHackQuest
" > DEBIAN/control

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

chmod +x etc/init.d/freehackquest-backend
chmod +x usr/bin/freehackquest-backend
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
