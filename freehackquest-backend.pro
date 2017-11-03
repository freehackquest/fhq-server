TEMPLATE = app
TARGET = freehackquest-backend

VERSION = 0.1.18
DEFINES += VERSION_STRING=\\\"0.1.18\\\"

CONFIG   += console
CONFIG   += release
CONFIG   -= app_bundle

QT += core sql network websockets
# QT -= gui

OBJECTS_DIR = tmp/
MOC_DIR = tmp/
RCC_DIR = tmp/
CONFIG += c++11 c++14

include(src/interfaces/interfaces.pri)
include(src/utils/utils.pri)
include(src/cmd/cmd.pri)
include(src/updates/updates.pri)
include(src/cache/cache.pri)
include(src/server/server.pri)
include(src/tasks/tasks.pri)
include(src/smtp/smtp.pri)
include(src/unit_tests/unit_tests.pri)

SOURCES += \
	src/main.cpp \

documentation.path = /usr/share/doc/freehackquest-backend
documentation.files = docs/*

INSTALLS += documentation

target.path = /usr/bin
INSTALLS += target

configfiles.files += data/config/*

configfiles.path = /usr/share/
configfiles.files = etc/freehackquest-backend/*
# yourset.extra = custom commands to run, eg. `touch somefile.txt`
INSTALLS += configfiles
