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
RESOURCES = src/fhq-server.qrc
# INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtZlib

# 3rd party
include(src/3rdParty/smtp/smtp.pri)
include(src/3rdParty/quazip-0.7.3.pri)
include(src/3rdParty/nlohmann.pri)

# other
include(src/employees/employees.pri)
include(src/models/models.pri)
include(src/interfaces/interfaces.pri)
include(src/utils/utils.pri)
include(src/cmd/cmd_handlers.pri)
include(src/cmd/classbook/cmd_classbook.pri)
include(src/cmd/events/cmd_events.pri)
include(src/cmd/games/cmd_games.pri)
include(src/cmd/quests/cmd_quests.pri)
include(src/cmd/mails/cmd_mails.pri)
include(src/cmd/users/cmd_users.pri)
include(src/cmd/server/cmd_server.pri)
include(src/cmd/support/cmd_support.pri)
include(src/updates/updates.pri)
include(src/cache/cache.pri)
include(src/server/server.pri)
include(src/tasks/tasks.pri)
include(src/unit_tests/unit_tests.pri)

SOURCES += \
        src/main.cpp \

target.path = /usr/bin
INSTALLS += target
