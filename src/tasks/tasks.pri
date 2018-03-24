
INCLUDEPATH += src/tasks/include
INCLUDEPATH += src/tasks

HEADERS += \
        src/tasks/include/add_public_events_task.h \
        src/tasks/include/update_max_score_game_task.h \
        src/tasks/include/update_quest_solved_task.h \
        src/tasks/include/update_user_location_task.h \
        src/tasks/include/update_user_rating_task.h \
	src/tasks/runtasks.h \
    $$PWD/include/mail_send_task.h

SOURCES += \
        src/tasks/sources/add_public_events_task.cpp \
        src/tasks/sources/update_max_score_game_task.cpp \
        src/tasks/sources/update_quest_solved_task.cpp \
        src/tasks/sources/update_user_rating_task.cpp \
        src/tasks/sources/update_user_location_task.cpp \
	src/tasks/runtasks.cpp \
    $$PWD/sources/mail_send_task.cpp

	

