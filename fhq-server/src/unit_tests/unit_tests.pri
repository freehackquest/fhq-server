
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/include/unit_test_levenshtein.h \
    $$PWD/include/unit_test_static_analizing_text.h \
    $$PWD/create_unit_tests.h \
    $$PWD/include/unit_test_merge_text.h

SOURCES += \
    $$PWD/sources/unit_test_levenshtein.cpp \
    $$PWD/sources/unit_test_static_analizing_text.cpp \
    $$PWD/create_unit_tests.cpp \
    $$PWD/sources/unit_test_merge_text.cpp
