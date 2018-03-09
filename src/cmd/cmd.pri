
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/create_cmd_handlers.h \
    $$PWD/include/cmd_addhint_handler.h \
    $$PWD/include/cmd_answerlist_handler.h \
    $$PWD/include/cmd_deletehint_handler.h \
    $$PWD/include/cmd_hints_handler.h \
    $$PWD/include/cmd_writeups_handler.h \

SOURCES += \
    $$PWD/create_cmd_handlers.cpp \
    $$PWD/sources/cmd_addhint_handler.cpp \
    $$PWD/sources/cmd_answerlist_handler.cpp \
    $$PWD/sources/cmd_deletehint_handler.cpp \
    $$PWD/sources/cmd_hints_handler.cpp \
    $$PWD/sources/cmd_writeups_handler.cpp \
