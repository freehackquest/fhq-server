TEMPLATE = app
TARGET = fhq-server

VERSION = 0.2.2
DEFINES += VERSION_STRING=\\\"0.2.2\\\"

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
include(src/updates/updates.pri)
include(src/server/server.pri)
include(src/tasks/tasks.pri)
include(src/unit_tests/unit_tests.pri)

SOURCES += \
        src/main.cpp \

target.path = /usr/bin
INSTALLS += target
