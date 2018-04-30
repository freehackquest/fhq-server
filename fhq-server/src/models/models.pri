
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/include/serversetthelper.h \
    $$PWD/include/cmd_input_def.h \
    $$PWD/include/model_leak.h \
    $$PWD/include/model_request.h \
    $$PWD/include/model_usertoken.h \
    $$PWD/include/model_server_config.h \
    $$PWD/include/model_command_access.h \
    $$PWD/include/model_lxd_container.h \
    $$PWD/include/model_command_input_def.h


SOURCES += \
    $$PWD/sources/serversetthelper.cpp \
    $$PWD/sources/cmd_input_def.cpp \
    $$PWD/sources/model_leak.cpp \
    $$PWD/sources/model_request.cpp \
    $$PWD/sources/model_usertoken.cpp \
    $$PWD/sources/model_server_config.cpp \
    $$PWD/sources/model_command_access.cpp \
    $$PWD/sources/model_lxd_container.cpp \
    $$PWD/sources/model_command_input_def.cpp


