HEADERS += \
	src/server/headers/usertoken.h \
	src/server/headers/server_config.h \
	src/server/headers/database_connection.h \
	src/server/impl/cmd_input_def.h \
	src/server/errors.h \
	src/server/error.h \
	src/server/websocketserver.h \

SOURCES += \
	src/server/websocketserver.cpp \
	src/server/impl/usertoken.cpp \
	src/server/impl/server_config.cpp \
	src/server/impl/database_connection.cpp \
	src/server/impl/cmd_input_def.cpp \
	src/server/errors.cpp \
	src/server/error.cpp \
