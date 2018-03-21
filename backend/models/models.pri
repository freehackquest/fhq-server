
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/include

HEADERS += \
    $$PWD/include/model_request.h \
    $$PWD/include/cmd_input_def.h \
    $$PWD/include/model_usertoken.h \
    $$PWD/include/model_server_config.h \
    $$PWD/include/model_command_access.h \
    $$PWD/include/model_lxd_container.h

SOURCES += \
    $$PWD/sources/model_request.cpp \
    $$PWD/sources/cmd_input_def.cpp \
    $$PWD/sources/model_usertoken.cpp \
    $$PWD/sources/model_server_config.cpp \
    $$PWD/sources/model_command_access.cpp \
    $$PWD/sources/model_lxd_container.cpp


