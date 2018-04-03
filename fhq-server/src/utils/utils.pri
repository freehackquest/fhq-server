
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/include/utils_levenshtein.h \
    $$PWD/include/utils_prepare_deb_package.h \
    $$PWD/include/utils_static_analizing_text.h \
    $$PWD/include/utils_create_config.h \
    $$PWD/include/utils_ini_parser.h \
    $$PWD/utils.h \
    $$PWD/include/utils_merge_text.h

SOURCES +=  \
    $$PWD/sources/utils_levenshtein.cpp \
    $$PWD/sources/utils_prepare_deb_package.cpp \
    $$PWD/sources/utils_static_analizing_text.cpp \
    $$PWD/sources/utils_create_config.cpp \
    $$PWD/sources/utils_ini_parser.cpp \
    $$PWD/sources/utils_merge_text.cpp

