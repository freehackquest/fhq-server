# FHQ Backend Deamon

Backend for freehackquest.com on Qt and WebSockets

## Requirements

        $ sudo apt install g++ make qtchooser qt5-default libqt5websockets5 libqt5websockets5-dev libqt5sql5-mysql

## Build

        $ qmake
        $ make

## configure

        $ cp ./etc/freehackquestd/conf.ini.example ./etc/freehackquestd/conf.ini
        $ sudo ln -s `pwd`/etc/freehackquestd /etc/freehackquestd
        $ sudo ln -s `pwd`/etc/init.d/freehackquestd /etc/init.d/freehackquestd
        $ sudo ln -s `pwd`/freehackquestd /usr/bin/freehackquestd
        $ sudo ln -s `pwd`/var/log/freehackquestd /var/log/freehackquestd
        $ sudo nano /etc/freehackquestd/conf.ini
        $ sudo update-rc.d -f freehackquestd remove
        $ sudo update-rc.d freehackquestd defaults
        $ sudo /etc/init.d/freehackquestd start

or you can use

		$ cp ./etc/freehackquestd/conf.ini.example ./etc/freehackquestd/conf.ini
		$ ./install.sh
		$ sudo nano /etc/freehackquestd/conf.ini
		$ service freehackquestd restart
