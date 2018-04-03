
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/include

HEADERS += \
    $$PWD/employees.h \
    $$PWD/employ_base.h \
    $$PWD/include/employ_settings.h \
    $$PWD/include/employ_server_config.h \
    $$PWD/include/employ_orchestra.h \
    $$PWD/include/employ_server_info.h

SOURCES += \
    $$PWD/employees.cpp \
    $$PWD/employ_base.cpp \
    $$PWD/sources/employ_settings.cpp \
    $$PWD/sources/employ_server_config.cpp \
    $$PWD/sources/employ_orchestra.cpp \
    $$PWD/sources/employ_server_info.cpp
