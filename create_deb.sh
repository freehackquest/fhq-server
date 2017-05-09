#!/bin/bash

name="freehackquest-backend"

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
mkdir -p "tmpdeb/usr/share/doc/$name"
mkdir -p "tmpdeb/usr/share/$name/libs"
cp -R "LICENSE" "tmpdeb/usr/share/doc/$name/copyright"

mkdir "tmpdeb/usr/bin"
cp "$name" "tmpdeb/usr/bin/$name"
strip -R -o "tmpdeb/usr/bin/$name" "tmpdeb/usr/bin/$name"
cp -R etc tmpdeb/
cp -R var tmpdeb/
cp -R usr tmpdeb/
rm -rf "tmpdeb/etc/$name/conf.ini"

./freehackquest-backend --prepare-deb

cd tmpdeb

find -type f | grep -re ~$ | while read f; do rm -rf "$f"; done

gzip -9 usr/share/doc/$name/changelog.Debian

# todo manual
# gzip -9 "usr/share/man/man1/freehackquest-backend.1"

# help: https://www.debian.org/doc/manuals/maint-guide/dreq.ru.html

if [ ! -d "DEBIAN" ]; then
	mkdir "DEBIAN"  
fi

# config files
echo "/etc/freehackquest-backend/conf.ini.example" >> DEBIAN/conffiles
echo "/etc/init.d/freehackquest-backend" >> DEBIAN/conffiles

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
rm -rf tmpdeb

#check
lintian *.deb > lintian.log
cat lintian.log
