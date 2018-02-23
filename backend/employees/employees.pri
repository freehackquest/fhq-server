
INCLUDEPATH += backend/employees
INCLUDEPATH += backend/interfaces
INCLUDEPATH += backend/employees/include

HEADERS += \
        backend/interfaces/iemploy.h \
        backend/employees/employees.h \
        backend/employees/include/employ_json.h \

SOURCES += \
	backend/employees/sources/employ_json.cpp \
	backend/employees/employees.cpp \
