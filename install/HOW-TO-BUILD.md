# HOW-TO-BUILD


## Ubuntu Requirements

```
$ sudo apt install g++ make qtchooser qt5-default libqt5websockets5 libqt5websockets5-dev libqt5sql5-mysql
```
New Requirements

```
$ sudo apt install libwebsockets-dev libcurl4-openssl-dev
$ sudo apt install zlibc zlib1g zlib1g-dev
$ sudo apt install libpng-dev
$ sudo apt install libmysqlclient-dev
```

## MacOS Requirements

! Mysql not supported in MacOS (non free)

So we can try install postgres install:

Remove previos version:

```
$ brew uninstall --force postgresql
$ rm -rf /usr/local/var/postgres
```
Install:

```
$ brew install postgres
```

Start PostgreSQL server:

```
$ pg_ctl -D /usr/local/var/postgres start
```

## Build 

### unit-tests - build & run

```
$ cd unit-tests
$ ./build_simple.sh
./unit_tests
```

### fhq-server - build & run

```
$ cd fhq-server
$ ./build_simple.sh
./fhq-server
```

# Mac OS requiremnets
