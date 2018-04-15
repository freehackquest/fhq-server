
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/include/cmd_classbook_localization_add_record_handler.h \
    $$PWD/include/cmd_classbook_localization_delete_record_handler.h \
    $$PWD/include/cmd_classbook_localization_update_record_handler.h \
    $$PWD/include/cmd_classbook_localization_info_handler.h \
    $$PWD/include/cmd_classbook_proposal_add_record_handler.h \
    $$PWD/include/cmd_classbook_proposal_delete_record_handler.h \
    $$PWD/include/cmd_classbook_proposal_info_handler.h \
    $$PWD/include/cmd_classbook_proposal_list_handler.h \
    $$PWD/include/cmd_classbook_proposal_prepare_merge_record.h


SOURCES += \
    $$PWD/sources/cmd_classbook_handler.cpp \
    $$PWD/sources/cmd_classbook_localization_add_record_handler.cpp \
    $$PWD/sources/cmd_classbook_localization_delete_record_handler.cpp \
    $$PWD/sources/cmd_classbook_localization_update_record_handler.cpp \
    $$PWD/sources/cmd_classbook_localization_info_handler.cpp \
    $$PWD/sources/cmd_classbook_proposal_add_record_handler.cpp \
    $$PWD/sources/cmd_classbook_proposal_delete_record_handler.cpp \
    $$PWD/sources/cmd_classbook_proposal_info_handler.cpp \
    $$PWD/sources/cmd_classbook_proposal_list_handler.cpp \
    $$PWD/sources/cmd_classbook_proposal_prepare_merge_record.cpp
