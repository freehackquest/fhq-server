# FHQ Backend Deamon

Backend for FreeHackQuest on Qt and WebSockets

## Pre-builds

Prebuild packages: http://dist.freehackquest.com/backend/

	$ sudo apt update
	$ wget "http://dist.freehackquest.com/backend/amd64/debian/8.0 (jessie)/freehackquest-backend_0.1.15_amd64.deb"
	$ su
	# apt install mysql-server
	# dpkg -i freehackquest-backend_0.1.15_amd64.deb
	# apt -f install
	# cp /etc/freehackquest-backend/conf.ini.example /etc/freehackquest-backend/conf.ini
	# nano /etc/freehackquest-backend/conf.ini
	
Run for check database configuration:

	# freehackquest-backend
	
Run like service (or just reboot system):

	# /etc/init.d/freehackquestd start

## Build

### Requirements

        $ sudo apt install g++ make qtchooser qt5-default libqt5websockets5 libqt5websockets5-dev libqt5sql5-mysql

### Build

        $ qmake
        $ make

### configure

        $ cp ./etc/freehackquest-backend/conf.ini.example ./etc/freehackquest-backend/conf.ini
        $ sudo ln -s `pwd`/etc/freehackquest-backend /etc/freehackquest-backend
        $ sudo ln -s `pwd`/etc/init.d/freehackquest-backend /etc/init.d/freehackquest-backend
        $ sudo ln -s `pwd`/freehackquest-backend /usr/bin/freehackquest-backend
        $ sudo ln -s `pwd`/var/log/freehackquest-backend /var/log/freehackquest-backend
        $ sudo nano /etc/freehackquest-backend/conf.ini
        $ sudo update-rc.d -f freehackquest-backend remove
        $ sudo update-rc.d freehackquest-backend defaults
        $ sudo /etc/init.d/freehackquest-backend start

Or you can use

		$ cp ./etc/freehackquest-backend/conf.ini.example ./etc/freehackquest-backend/conf.ini
		$ ./install.sh
		$ sudo nano /etc/freehackquest-backend/conf.ini
		$ service freehackquest-backend restart
		
## Doxygen generate documentation

### requariments

	$ sudo apt install doxygen graphviz
	
### Build documentation

	$ doxygen doxygen.conf
		
## Other

Contains source code to compile: https://github.com/bluetiger9/SmtpClient-for-Qt


