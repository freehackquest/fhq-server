
INCLUDEPATH += src/cmd_users/include
INCLUDEPATH += src/cmd_users

HEADERS += \
    src/cmd_users/create_cmd_users_handlers.h \
    src/cmd_users/include/cmd_user_handler.h \
    src/cmd_users/include/cmd_user_reset_password_handler.h \
    src/cmd_users/include/cmd_users_handler.h \
    src/cmd_users/include/cmd_user_skills_handler.h \
    src/cmd_users/include/cmd_user_update_handler.h \
    src/cmd_users/include/cmd_user_change_password_handler.h \
    src/cmd_users/include/cmd_update_user_location_handler.h \

SOURCES += \
    src/cmd_users/create_cmd_users_handlers.cpp \
    src/cmd_users/sources/cmd_user_handler.cpp \
    src/cmd_users/sources/cmd_user_reset_password_handler.cpp \
    src/cmd_users/sources/cmd_users_handler.cpp \
    src/cmd_users/sources/cmd_user_skills_handler.cpp \
    src/cmd_users/sources/cmd_user_update_handler.cpp \
    src/cmd_users/sources/cmd_user_change_password_handler.cpp \
    src/cmd_users/sources/cmd_update_user_location_handler.cpp \
