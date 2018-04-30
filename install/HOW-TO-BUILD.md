# HOW-TO-BUILD

## Requirements Qt

```
$ sudo apt install g++ make qtchooser qt5-default libqt5websockets5 libqt5websockets5-dev libqt5sql5-mysql
```

## New Requirements

```
$ sudo apt install libwebsockets-dev libcurl4-openssl-dev
$ sudo apt install zlibc zlib1g zlib1g-dev
$ sudo apt install libpng-dev
```

## Building CMake

```
$ cd fhq-server
$ mkdir -p build
$ cd build
$ cmake ..
$ make
```

Or simple:

```
$ cd fhq-server
$ ./build_simple.sh
```

## Clean

```
$ ./clean.sh
```

## Install 
```
$ make && sudo make install
```
