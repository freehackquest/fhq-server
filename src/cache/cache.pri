
INCLUDEPATH += src/cache
INCLUDEPATH += src/cache/include

HEADERS += \
	src/cache/create_memory_cache.h \
        src/cache/include/memory_cache_scoreboard.h \
        src/cache/include/memory_cache_serverinfo.h \
        src/cache/include/memory_cache_serversettings.h \
        src/cache/include/serversetthelper.h \

SOURCES += \
	src/cache/create_memory_cache.cpp \
        src/cache/sources/memory_cache_scoreboard.cpp \
        src/cache/sources/memory_cache_serverinfo.cpp \
        src/cache/sources/memory_cache_serversettings.cpp \
        src/cache/sources/serversetthelper.cpp \
