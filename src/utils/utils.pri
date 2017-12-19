
INCLUDEPATH += src/utils/include
INCLUDEPATH += src/utils

HEADERS += \
        src/utils/include/utils_levenshtein.h \
        src/utils/include/utils_prepare_deb_package.h \
        src/utils/include/utils_static_analizing_text.h \
        src/utils/include/utils_create_config.h \
        src/utils/include/utils_ini_parser.h \
        src/utils/utils.h \
    $$PWD/include/utils_merge_text.h

SOURCES += \
        src/utils/sources/utils_levenshtein.cpp \
        src/utils/sources/utils_prepare_deb_package.cpp \
        src/utils/sources/utils_static_analizing_text.cpp \
        src/utils/sources/utils_create_config.cpp \
        src/utils/sources/utils_ini_parser.cpp \
    $$PWD/sources/utils_merge_text.cpp
