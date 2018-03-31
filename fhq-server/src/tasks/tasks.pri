
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/include/add_public_events_task.h \
    $$PWD/include/update_max_score_game_task.h \
    $$PWD/include/update_quest_solved_task.h \
    $$PWD/include/update_user_location_task.h \
    $$PWD/include/update_user_rating_task.h \
    $$PWD/runtasks.h \
    $$PWD/include/mail_send_task.h

SOURCES += \
    $$PWD/sources/add_public_events_task.cpp \
    $$PWD/sources/update_max_score_game_task.cpp \
    $$PWD/sources/update_quest_solved_task.cpp \
    $$PWD/sources/update_user_rating_task.cpp \
    $$PWD/sources/update_user_location_task.cpp \
    $$PWD/runtasks.cpp \
    $$PWD/sources/mail_send_task.cpp

	

