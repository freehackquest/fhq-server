
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/include

HEADERS += \
    $$PWD/employees.h \
    $$PWD/include/employ_json.h \
    $$PWD/include/employ_settings.h \
    $$PWD/include/employ_server_config.h \
    $$PWD/include/employ_orchestra.h

SOURCES += \
    $$PWD/employees.cpp \
    $$PWD/sources/employ_json.cpp \
    $$PWD/sources/employ_settings.cpp \
    $$PWD/sources/employ_server_config.cpp \
    $$PWD/sources/employ_orchestra.cpp

