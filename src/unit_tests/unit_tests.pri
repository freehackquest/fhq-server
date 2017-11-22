
INCLUDEPATH += src/unit_tests/include
INCLUDEPATH += src/unit_tests

HEADERS += \
        src/unit_tests/include/unit_test_levenshtein.h \
        src/unit_tests/include/unit_static_analizing_text.h \
        src/unit_tests/create_unit_tests.h \
    $$PWD/include/unit_test_merge_text.h

SOURCES += \
        src/unit_tests/sources/unit_test_levenshtein.cpp \
        src/unit_tests/sources/unit_static_analizing_text.cpp \
        src/unit_tests/create_unit_tests.cpp \
    $$PWD/sources/unit_test_merge_text.cpp
