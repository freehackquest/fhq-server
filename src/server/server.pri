HEADERS += \
	src/server/headers/usertoken.h \
	src/server/headers/server_config.h \
	src/server/headers/database_connection.h \
	src/server/headers/cmd_input_def.h \
	src/server/headers/errors.h \
	src/server/headers/error.h \
	src/server/headers/websocketserver.h \

SOURCES += \
	src/server/impl/websocketserver.cpp \
	src/server/impl/usertoken.cpp \
	src/server/impl/server_config.cpp \
	src/server/impl/database_connection.cpp \
	src/server/impl/cmd_input_def.cpp \
	src/server/impl/errors.cpp \
	src/server/impl/error.cpp \
