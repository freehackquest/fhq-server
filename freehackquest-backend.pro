TEMPLATE = app
TARGET = freehackquest-backend

VERSION = 0.1.17
DEFINES += VERSION_STRING=\\\"0.1.17\\\"

QT += core sql network websockets
QT -= gui

CONFIG   += console
CONFIG   += release
CONFIG   -= app_bundle
OBJECTS_DIR = tmp/
MOC_DIR = tmp/
RCC_DIR = tmp/
CONFIG += c++11 c++14

include(src/interfaces/interfaces.pri)
include(src/cmd/handlers.pri)
include(src/updates/updates.pri)
include(src/cache/cache.pri)
include(src/server/server.pri)
include(src/tasks/tasks.pri)
include(src/smtp/smtp.pri)

SOURCES += \
	src/prepare_tmp_deb_package.cpp \
	src/main.cpp \

HEADERS += \
	src/prepare_tmp_deb_package.h \
