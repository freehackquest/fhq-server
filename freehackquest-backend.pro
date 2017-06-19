TEMPLATE = app
TARGET = freehackquest-backend

QT += core sql network websockets
QT -= gui

CONFIG   += console
CONFIG   += release
CONFIG   -= app_bundle
OBJECTS_DIR = tmp/
MOC_DIR = tmp/
RCC_DIR = tmp/
CONFIG += c++11 c++14

INCLUDEPATH += src/interfaces
INCLUDEPATH += src/server/headers
INCLUDEPATH += src/cache/headers
INCLUDEPATH += src/cache
INCLUDEPATH += src/updates
INCLUDEPATH += src/cmd

include(src/interfaces/interfaces.pri)
include(src/cmd/handlers.pri)
include(src/updates/updates.pri)
include(src/cache/cache.pri)
include(src/server/server.pri)

SOURCES += \
	src/prepare_tmp_deb_package.cpp \
	src/main.cpp \
	src/smtp/smtp.cpp \
	src/tasks/update_user_location_task.cpp \

HEADERS += \
	src/prepare_tmp_deb_package.h \
	src/smtp/smtp.h \
	src/tasks/update_user_location_task.h \
	
	
