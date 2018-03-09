
INCLUDEPATH += $$PWD/include

HEADERS += \
    $$PWD/include/cmd_public_info_handler.h \
    $$PWD/include/cmd_server_info_handler.h \
    $$PWD/include/cmd_server_settings_handler.h \
    $$PWD/include/cmd_server_settings_update_handler.h \
    $$PWD/include/cmd_handler_server_api.h

SOURCES += \
    $$PWD/sources/cmd_public_info_handler.cpp \
    $$PWD/sources/cmd_server_info_handler.cpp \
    $$PWD/sources/cmd_server_settings_handler.cpp \
    $$PWD/sources/cmd_server_settings_update_handler.cpp \
    $$PWD/sources/cmd_handler_server_api_handler.cpp
