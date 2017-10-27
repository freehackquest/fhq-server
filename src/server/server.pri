
INCLUDEPATH += src/server/include
INCLUDEPATH += src/server

HEADERS += \
        src/server/include/usertoken.h \
        src/server/include/server_config.h \
        src/server/include/database_connection.h \
        src/server/include/cmd_input_def.h \
        src/server/include/errors.h \
        src/server/include/error.h \
        src/server/include/log.h \
        src/server/include/websocketserver.h \

SOURCES += \
        src/server/sources/websocketserver.cpp \
        src/server/sources/usertoken.cpp \
        src/server/sources/server_config.cpp \
        src/server/sources/database_connection.cpp \
        src/server/sources/cmd_input_def.cpp \
        src/server/sources/errors.cpp \
        src/server/sources/error.cpp \
        src/server/sources/log.cpp \
