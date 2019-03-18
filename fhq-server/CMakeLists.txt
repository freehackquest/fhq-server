cmake_minimum_required(VERSION 3.0)

project(fhq-server)

add_definitions(-DFHQSRV_VERSION="0.2.20")
add_definitions(-DFHQSRV_APP_NAME="fhq-server")

set (CMAKE_AUTOMOC ON)
set (CMAKE_CXX_STANDARD 11)
set (EXECUTABLE_OUTPUT_PATH ${fhq-server_SOURCE_DIR})
set (QTCORE_LIBRARIES ${Qt5Core_LIBRARIES})
include_directories(${Qt5Core_INCLUDE_DIRS})

set (FHQSRV_LIBRARIES "")
set (FHQSRV_INCLUDE_DIRS "")
set (FHQSRV_SOURCES "")

# Sources

# include header dirs
list (APPEND FHQSRV_INCLUDE_DIRS "src/3rdParty/nlohmann")
list (APPEND FHQSRV_INCLUDE_DIRS "src/3rdParty/smallsha1")
list (APPEND FHQSRV_INCLUDE_DIRS "src/3rdParty/md5")
list (APPEND FHQSRV_INCLUDE_DIRS "src/3rdParty/quazip-0.7.3/quazip")
list (APPEND FHQSRV_INCLUDE_DIRS "src/unit_tests/include")
list (APPEND FHQSRV_INCLUDE_DIRS "src/unit_tests")
list (APPEND FHQSRV_INCLUDE_DIRS "src/utils")
list (APPEND FHQSRV_INCLUDE_DIRS "src/utils/include")
list (APPEND FHQSRV_INCLUDE_DIRS "src/employees")
list (APPEND FHQSRV_INCLUDE_DIRS "src/employees/include")
list (APPEND FHQSRV_INCLUDE_DIRS "src/models")
list (APPEND FHQSRV_INCLUDE_DIRS "src/models/include")
list (APPEND FHQSRV_INCLUDE_DIRS "src/server")
list (APPEND FHQSRV_INCLUDE_DIRS "src/server/include")
list (APPEND FHQSRV_INCLUDE_DIRS "src/interfaces")
list (APPEND FHQSRV_INCLUDE_DIRS "src/tasks")
list (APPEND FHQSRV_INCLUDE_DIRS "src/updates")
list (APPEND FHQSRV_INCLUDE_DIRS "src/tasks/include")
list (APPEND FHQSRV_INCLUDE_DIRS "src/tasks")
list (APPEND FHQSRV_INCLUDE_DIRS "src/storages")
list (APPEND FHQSRV_INCLUDE_DIRS "src/storages/updates")
list (APPEND FHQSRV_INCLUDE_DIRS "src/updates")
list (APPEND FHQSRV_INCLUDE_DIRS "src/storages/updates/include")
list (APPEND FHQSRV_INCLUDE_DIRS "src/cmd")
list (APPEND FHQSRV_INCLUDE_DIRS "src/cmd/include")
list (APPEND FHQSRV_INCLUDE_DIRS "src/cmd/classbook/include")
list (APPEND FHQSRV_INCLUDE_DIRS "src/web-server")
list (APPEND FHQSRV_INCLUDE_DIRS "src/web-server/core")

# json
list (APPEND FHQSRV_SOURCES "src/3rdParty/nlohmann/json.hpp")

# small md5
list (APPEND FHQSRV_SOURCES "src/3rdParty/md5/md5.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/md5/md5.cpp")

# small sha1
list (APPEND FHQSRV_SOURCES "src/3rdParty/smallsha1/sha1.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/smallsha1/sha1.cpp")
list (APPEND FHQSRV_SOURCES "src/3rdParty/smallsha1/sha1_wrapper.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/smallsha1/sha1_wrapper.cpp")

# interfaces
list (APPEND FHQSRV_SOURCES "src/interfaces/iserver.h")
list (APPEND FHQSRV_SOURCES "src/interfaces/iuser.h")
list (APPEND FHQSRV_SOURCES "src/interfaces/iusertoken.h")
list (APPEND FHQSRV_SOURCES "src/interfaces/iwebsocketserver.h")

# quazip
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/crypt.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/ioapi.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/JlCompress.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quaadler32.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quachecksum32.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quacrc32.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quagzipfile.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quaziodevice.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipdir.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipfile.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipfileinfo.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazip_global.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazip.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipnewinfo.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/unzip.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/zip.h")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/qioapi.cpp")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/JlCompress.cpp")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quaadler32.cpp")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quacrc32.cpp")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quagzipfile.cpp")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quaziodevice.cpp")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazip.cpp")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipdir.cpp")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipfile.cpp")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipfileinfo.cpp")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipnewinfo.cpp")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/unzip.c")
list (APPEND FHQSRV_SOURCES "src/3rdParty/quazip-0.7.3/quazip/zip.c")

# cmd
list (APPEND FHQSRV_SOURCES "src/cmd/cmd_handlers_classbook.h")
list (APPEND FHQSRV_SOURCES "src/cmd/cmd_handlers_classbook.cpp")
list (APPEND FHQSRV_SOURCES "src/cmd/cmd_handlers_communication.h")
list (APPEND FHQSRV_SOURCES "src/cmd/cmd_handlers_communication.cpp")
list (APPEND FHQSRV_SOURCES "src/cmd/cmd_handlers_quests_writeups.h")
list (APPEND FHQSRV_SOURCES "src/cmd/cmd_handlers_quests_writeups.cpp")
list (APPEND FHQSRV_SOURCES "src/cmd/cmd_handlers.h")
list (APPEND FHQSRV_SOURCES "src/cmd/cmd_handlers.cpp")
list (APPEND FHQSRV_SOURCES "src/cmd/include/cmd_handlers_users.h")
list (APPEND FHQSRV_SOURCES "src/cmd/sources/cmd_handlers_users.cpp")
list (APPEND FHQSRV_SOURCES "src/cmd/include/cmd_handlers_events.h")
list (APPEND FHQSRV_SOURCES "src/cmd/sources/cmd_handlers_events.cpp")
list (APPEND FHQSRV_SOURCES "src/cmd/include/cmd_handlers_games.h")
list (APPEND FHQSRV_SOURCES "src/cmd/sources/cmd_handlers_games.cpp")
list (APPEND FHQSRV_SOURCES "src/cmd/include/cmd_handlers_mails.h")
list (APPEND FHQSRV_SOURCES "src/cmd/sources/cmd_handlers_mails.cpp")
list (APPEND FHQSRV_SOURCES "src/cmd/include/cmd_handlers_quests.h")
list (APPEND FHQSRV_SOURCES "src/cmd/sources/cmd_handlers_quests.cpp")
list (APPEND FHQSRV_SOURCES "src/cmd/include/cmd_handlers_server.h")
list (APPEND FHQSRV_SOURCES "src/cmd/sources/cmd_handlers_server.cpp")
list (APPEND FHQSRV_SOURCES "src/cmd/include/cmd_handlers_support.h")
list (APPEND FHQSRV_SOURCES "src/cmd/sources/cmd_handlers_support.cpp")
list (APPEND FHQSRV_SOURCES "src/cmd/include/cmd_handlers_lxd.h")
list (APPEND FHQSRV_SOURCES "src/cmd/sources/cmd_handlers_lxd.cpp")
list (APPEND FHQSRV_SOURCES "src/cmd/include/cmd_handlers_leaks.h")
list (APPEND FHQSRV_SOURCES "src/cmd/sources/cmd_handlers_leaks.cpp")

# models
list (APPEND FHQSRV_SOURCES "src/models/model_notification.cpp")
list (APPEND FHQSRV_SOURCES "src/models/model_notification.h")
list (APPEND FHQSRV_SOURCES "src/models/model_request.cpp")
list (APPEND FHQSRV_SOURCES "src/models/model_request.h")
list (APPEND FHQSRV_SOURCES "src/models/include/model_database_connection.h")
list (APPEND FHQSRV_SOURCES "src/models/sources/model_database_connection.cpp")
list (APPEND FHQSRV_SOURCES "src/models/include/model_server_sett_helper.h")
list (APPEND FHQSRV_SOURCES "src/models/sources/model_server_sett_helper.cpp")
list (APPEND FHQSRV_SOURCES "src/models/include/cmd_input_def.h")
list (APPEND FHQSRV_SOURCES "src/models/include/model_leak.h")
list (APPEND FHQSRV_SOURCES "src/models/sources/model_leak.cpp")
list (APPEND FHQSRV_SOURCES "src/models/include/model_game.h")
list (APPEND FHQSRV_SOURCES "src/models/sources/model_game.cpp")
list (APPEND FHQSRV_SOURCES "src/models/include/model_usertoken.h")
list (APPEND FHQSRV_SOURCES "src/models/include/model_command_input_def.h")
list (APPEND FHQSRV_SOURCES "src/models/include/model_command_access.h")
list (APPEND FHQSRV_SOURCES "src/models/sources/cmd_input_def.cpp")
list (APPEND FHQSRV_SOURCES "src/models/sources/model_usertoken.cpp")
list (APPEND FHQSRV_SOURCES "src/models/sources/model_command_input_def.cpp")
list (APPEND FHQSRV_SOURCES "src/models/sources/model_command_access.cpp")
list (APPEND FHQSRV_SOURCES "src/models/include/model_lxd_container.h")
list (APPEND FHQSRV_SOURCES "src/models/sources/model_lxd_container.cpp")

# storages
list (APPEND FHQSRV_SOURCES "src/storages/storage.h")
list (APPEND FHQSRV_SOURCES "src/storages/storage.cpp")
list (APPEND FHQSRV_SOURCES "src/storages/storages.h")
list (APPEND FHQSRV_SOURCES "src/storages/storages.cpp")


# TODO include only if libmysqlclient exists 
list (APPEND FHQSRV_INCLUDE_DIRS "src/storages/mysql")
list (APPEND FHQSRV_SOURCES "src/storages/mysql/mysql_storage.cpp")
list (APPEND FHQSRV_SOURCES "src/storages/mysql/mysql_storage.h")

# utils
list (APPEND FHQSRV_SOURCES "src/utils/export_libfhqcli_py.h")
list (APPEND FHQSRV_SOURCES "src/utils/export_libfhqcli_py.cpp")
list (APPEND FHQSRV_SOURCES "src/utils/export_libfhqcli_web_js.h")
list (APPEND FHQSRV_SOURCES "src/utils/export_libfhqcli_web_js.cpp")
list (APPEND FHQSRV_SOURCES "src/utils/fs.h")
list (APPEND FHQSRV_SOURCES "src/utils/fs.cpp")
list (APPEND FHQSRV_SOURCES "src/utils/help_parse_args.h")
list (APPEND FHQSRV_SOURCES "src/utils/help_parse_args.cpp")
list (APPEND FHQSRV_SOURCES "src/utils/logger.h")
list (APPEND FHQSRV_SOURCES "src/utils/logger.cpp")
list (APPEND FHQSRV_SOURCES "src/utils/parse_config.h")
list (APPEND FHQSRV_SOURCES "src/utils/parse_config.cpp")
list (APPEND FHQSRV_SOURCES "src/utils/str.h")
list (APPEND FHQSRV_SOURCES "src/utils/str.cpp")
list (APPEND FHQSRV_SOURCES "src/utils/ts.h")
list (APPEND FHQSRV_SOURCES "src/utils/ts.cpp")
list (APPEND FHQSRV_SOURCES "src/utils/utils_merge_text.h")
list (APPEND FHQSRV_SOURCES "src/utils/utils_merge_text.cpp")
list (APPEND FHQSRV_SOURCES "src/utils/utils.h")
list (APPEND FHQSRV_SOURCES "src/utils/include/utils_export_list_of_handlers.h")
list (APPEND FHQSRV_SOURCES "src/utils/sources/utils_export_list_of_handlers.cpp")
list (APPEND FHQSRV_SOURCES "src/utils/include/utils_levenshtein.h")
list (APPEND FHQSRV_SOURCES "src/utils/include/utils_prepare_deb_package.h")
list (APPEND FHQSRV_SOURCES "src/utils/include/utils_static_analizing_text.h")
list (APPEND FHQSRV_SOURCES "src/utils/include/utils_lxd.h")
list (APPEND FHQSRV_SOURCES "src/utils/sources/utils_levenshtein.cpp")
list (APPEND FHQSRV_SOURCES "src/utils/sources/utils_prepare_deb_package.cpp")
list (APPEND FHQSRV_SOURCES "src/utils/sources/utils_static_analizing_text.cpp")
list (APPEND FHQSRV_SOURCES "src/utils/sources/utils_lxd.cpp")

# server
list (APPEND FHQSRV_SOURCES "src/server/include/errors.h")
list (APPEND FHQSRV_SOURCES "src/server/include/error.h")
list (APPEND FHQSRV_SOURCES "src/server/include/websocketserver.h")
list (APPEND FHQSRV_SOURCES "src/server/sources/websocketserver.cpp")
list (APPEND FHQSRV_SOURCES "src/server/sources/errors.cpp")
list (APPEND FHQSRV_SOURCES "src/server/sources/error.cpp")

# employees
list (APPEND FHQSRV_SOURCES "src/employees/employees.h")
list (APPEND FHQSRV_SOURCES "src/employees/employees.cpp")
list (APPEND FHQSRV_SOURCES "src/employees/include/employ_images.h")
list (APPEND FHQSRV_SOURCES "src/employees/sources/employ_images.cpp")
list (APPEND FHQSRV_SOURCES "src/employees/include/employ_games.h")
list (APPEND FHQSRV_SOURCES "src/employees/sources/employ_games.cpp")
list (APPEND FHQSRV_SOURCES "src/employees/include/employ_notify.h")
list (APPEND FHQSRV_SOURCES "src/employees/sources/employ_notify.cpp")
list (APPEND FHQSRV_SOURCES "src/employees/include/employ_quests.h")
list (APPEND FHQSRV_SOURCES "src/employees/sources/employ_quests.cpp")
list (APPEND FHQSRV_SOURCES "src/employees/include/employ_settings.h")
list (APPEND FHQSRV_SOURCES "src/employees/include/employ_server_config.h")
list (APPEND FHQSRV_SOURCES "src/employees/include/employ_server_info.h")
list (APPEND FHQSRV_SOURCES "src/employees/include/employ_database.h")
list (APPEND FHQSRV_SOURCES "src/employees/include/employ_scoreboard.h")
list (APPEND FHQSRV_SOURCES "src/employees/include/employ_leaks.h")
list (APPEND FHQSRV_SOURCES "src/employees/sources/employ_settings.cpp")
list (APPEND FHQSRV_SOURCES "src/employees/sources/employ_server_config.cpp")
list (APPEND FHQSRV_SOURCES "src/employees/sources/employ_server_info.cpp")
list (APPEND FHQSRV_SOURCES "src/employees/sources/employ_database.cpp")
list (APPEND FHQSRV_SOURCES "src/employees/sources/employ_scoreboard.cpp")
list (APPEND FHQSRV_SOURCES "src/employees/sources/employ_leaks.cpp")
list (APPEND FHQSRV_SOURCES "src/employees/include/employ_ws_server.h")
list (APPEND FHQSRV_SOURCES "src/employees/sources/employ_ws_server.cpp")
list (APPEND FHQSRV_SOURCES "src/employees/include/employ_orchestra.h")
list (APPEND FHQSRV_SOURCES "src/employees/sources/employ_orchestra.cpp")

# tasks
list (APPEND FHQSRV_SOURCES "src/tasks/runtasks.h")
list (APPEND FHQSRV_SOURCES "src/tasks/runtasks.cpp")
list (APPEND FHQSRV_SOURCES "src/tasks/include/mail_send_task.h")
list (APPEND FHQSRV_SOURCES "src/tasks/sources/mail_send_task.cpp")
list (APPEND FHQSRV_SOURCES "src/tasks/include/notify_to_all_task.h")
list (APPEND FHQSRV_SOURCES "src/tasks/sources/notify_to_all_task.cpp")

list (APPEND FHQSRV_SOURCES "src/tasks/include/add_public_events_task.h")
list (APPEND FHQSRV_SOURCES "src/tasks/include/update_max_score_game_task.h")
list (APPEND FHQSRV_SOURCES "src/tasks/include/update_quest_solved_task.h")
list (APPEND FHQSRV_SOURCES "src/tasks/include/update_user_location_task.h")
list (APPEND FHQSRV_SOURCES "src/tasks/include/update_user_rating_task.h")
list (APPEND FHQSRV_SOURCES "src/tasks/include/lxd_async_operation_task.h")
list (APPEND FHQSRV_SOURCES "src/tasks/sources/add_public_events_task.cpp")
list (APPEND FHQSRV_SOURCES "src/tasks/sources/update_max_score_game_task.cpp")
list (APPEND FHQSRV_SOURCES "src/tasks/sources/update_quest_solved_task.cpp")
list (APPEND FHQSRV_SOURCES "src/tasks/sources/update_user_rating_task.cpp")
list (APPEND FHQSRV_SOURCES "src/tasks/sources/update_user_location_task.cpp")
list (APPEND FHQSRV_SOURCES "src/tasks/sources/lxd_async_operation_task.cpp")

# updates
list (APPEND FHQSRV_SOURCES "src/storages/updates/updates_init_database.h")
list (APPEND FHQSRV_SOURCES "src/storages/updates/updates_init_database.cpp")
list (APPEND FHQSRV_SOURCES "src/storages/updates/update0100.h")
list (APPEND FHQSRV_SOURCES "src/storages/updates/update0100.cpp")

# web-server
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_deque_requests.cpp")
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_deque_requests.h")
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_handler_base.cpp")
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_handler_base.h")
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_handlers.cpp")
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_handlers.h")
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_request.cpp")
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_request.h")
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_response.cpp")
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_response.h")
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_server.cpp")
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_server.h")
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_thread_worker.cpp")
list (APPEND FHQSRV_SOURCES "src/web-server/core/light_http_thread_worker.h")
list (APPEND FHQSRV_SOURCES "src/web-server/http_handler_web_admin_folder.cpp")
list (APPEND FHQSRV_SOURCES "src/web-server/http_handler_web_admin_folder.h")


# main
list (APPEND FHQSRV_SOURCES "src/main.cpp")


############################
#####
#foreach (_headerFile ${FHQSRV_HEADERS})
#    get_filename_component(_dir ${_headerFile} PATH)
#    list (APPEND FHQSRV_INCLUDE_DIRS ${_dir})
#endforeach()
#list(REMOVE_DUPLICATES FHQSRV_INCLUDE_DIRS)
#####
############################


############################
##### ZLIB
find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    list (APPEND FHQSRV_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    list (APPEND FHQSRV_LIBRARIES ${ZLIB_LIBRARIES})
endif( ZLIB_FOUND )
#####
############################


############################
##### LIBPNG
find_package(PNG REQUIRED)
if (NOT PNG_FOUND)
    message(FATAL_ERROR "You don't seem to have libpng development libraries installed (apt isntall libpng-dev)")
else ()
    list (APPEND FHQSRV_INCLUDE_DIRS ${PNG_INCLUDE_DIR})
    list (APPEND FHQSRV_LIBRARIES ${PNG_LIBRARY})
endif ()
#####
############################

############################
##### CURL
FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  list (APPEND FHQSRV_INCLUDE_DIRS ${CURL_INCLUDE_DIR})
  list (APPEND FHQSRV_LIBRARIES ${CURL_LIBRARIES})
ELSE(CURL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
ENDIF(CURL_FOUND)
#####
############################


############################
##### mysql 
# Find and make sure the system have the header file
find_path(MYSQL_HEADER mysql/mysql.h)
if(MYSQL_HEADER STREQUAL "MYSQL_HEADER-NOTFOUND")
    message(FATAL_ERROR "Could not find the mysql/mysql.h header file: (ubuntu: apt install libmysqlclient-dev) or (debian: apt install default-libmariadbclient-dev and link: ln -s /usr/lib/x86_64-linux-gnu/pkgconfig/mariadb.pc /usr/lib/x86_64-linux-gnu/pkgconfig/mysqlclient.pc)")
endif()

include(FindPkgConfig)
pkg_check_modules(LIBMYSQLCLIENT REQUIRED mysqlclient)

foreach(FLAG ${LIBMYSQLCLIENT_CFLAGS_OTHER})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}")
endforeach()

list (APPEND FHQSRV_INCLUDE_DIRS ${LIBMYSQLCLIENT_LIBRARY_DIRS})
list (APPEND FHQSRV_LIBRARIES ${LIBMYSQLCLIENT_LIBRARIES})
#####
############################

include_directories(${FHQSRV_INCLUDE_DIRS})

find_package (Qt5Core)
find_package (Qt5Sql)
find_package (Qt5Network)
find_package (Qt5WebSockets)

add_executable (fhq-server ${FHQSRV_SOURCES})

qt5_use_modules(fhq-server Core Network Sql WebSockets)
target_link_libraries(fhq-server -lpthread ${FHQSRV_LIBRARIES} )

# target_link_libraries(fhq-server Qt5::Core)

install(
    TARGETS
        fhq-server
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_PREFIX}/bin
)

# install(FILES install_files/fhq-server.conf.sample DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/fhq-server COMPONENT config)
# install(FILES install_files/fhq-server.service DESTINATION ${CMAKE_INSTALL_PREFIX}/../etc/systemd/system COMPONENT config)
# install(FILES install_files/fhq-server.postinst DESTINATION ${CMAKE_INSTALL_PREFIX}/../var/lib/dpkg/info COMPONENT config)
# install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/share/fhq-server)
# install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/log/fhq-server)
