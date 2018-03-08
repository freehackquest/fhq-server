TEMPLATE = app
TARGET = freehackquest-backend

VERSION = 0.1.19
DEFINES += VERSION_STRING=\\\"0.1.19\\\"

CONFIG   += console
CONFIG   += release
CONFIG   -= app_bundle

QT += core sql network websockets
# QT -= gui

OBJECTS_DIR = tmp/
MOC_DIR = tmp/
RCC_DIR = tmp/
CONFIG += c++11 c++14
LIBS = -lz
RESOURCES = freehackquest-backend.qrc
# INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtZlib

# 3rd party
include(backend/3rdParty/smtp/smtp.pri)
include(backend/3rdParty/quazip-0.7.3.pri)

# other
include(backend/employees/employees.pri)
include(backend/models/models.pri)
include(backend/interfaces/interfaces.pri)
include(src/utils/utils.pri)
include(src/cmd/cmd.pri)
include(backend/cmd/cmd_handlers.pri)
include(src/cmd_users/cmd_users.pri)
include(backend/cmd/games/cmd_games.pri)
include(backend/cmd/quests/cmd_quests.pri)
include(backend/cmd/mails/cmd_mails.pri)
include(src/updates/updates.pri)
include(src/cache/cache.pri)
include(src/server/server.pri)
include(src/tasks/tasks.pri)
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
