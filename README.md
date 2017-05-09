# FHQ Backend Deamon

Backend for FreeHackQuest on Qt and WebSockets


## Pre-builds

Prebuild packages: http://dist.freehackquest.com/backend/

	$ sudo apt update
	$ wget http://dist.freehackquest.com/backend/debian-x64/freehackquest-backend_0.1.14_amd64.deb
	$ su
	# apt install mysql-server
	# dpkg -i freehackquest-backend_0.1.14_amd64.deb
	# apt -f install
	# cp /etc/freehackquest-backend/conf.ini.example /etc/freehackquest-backend/conf.ini
	# nano /etc/freehackquest-backend/conf.ini
	
Run for check database configuration:

	# freehackquest -d
	
Run like service (or just reboot system):

	# /etc/init.d/freehackquestd start

## Build

### Requirements

        $ sudo apt install g++ make qtchooser qt5-default libqt5websockets5 libqt5websockets5-dev libqt5sql5-mysql

### Build

        $ qmake
        $ make

### configure

        $ cp ./etc/freehackquestd/conf.ini.example ./etc/freehackquestd/conf.ini
        $ sudo ln -s `pwd`/etc/freehackquestd /etc/freehackquestd
        $ sudo ln -s `pwd`/etc/init.d/freehackquestd /etc/init.d/freehackquestd
        $ sudo ln -s `pwd`/freehackquestd /usr/bin/freehackquestd
        $ sudo ln -s `pwd`/var/log/freehackquestd /var/log/freehackquestd
        $ sudo nano /etc/freehackquestd/conf.ini
        $ sudo update-rc.d -f freehackquestd remove
        $ sudo update-rc.d freehackquestd defaults
        $ sudo /etc/init.d/freehackquestd start

Or you can use

		$ cp ./etc/freehackquestd/conf.ini.example ./etc/freehackquestd/conf.ini
		$ ./install.sh
		$ sudo nano /etc/freehackquestd/conf.ini
		$ service freehackquestd restart
