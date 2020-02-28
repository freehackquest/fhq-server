cmake_minimum_required(VERSION 3.0)

project(fhq-server)

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

add_definitions(-DFHQSRV_VERSION="0.2.25")
add_definitions(-DFHQSRV_APP_NAME="fhq-server")

set (CMAKE_AUTOMOC ON)
set (CMAKE_CXX_STANDARD 11)
set (EXECUTABLE_OUTPUT_PATH ${fhq-server_SOURCE_DIR})
set (QTCORE_LIBRARIES ${Qt5Core_LIBRARIES})
include_directories(${Qt5Core_INCLUDE_DIRS})

# Sources

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")
list (APPEND WSJCPP_INCLUDE_DIRS "src/core")
list (APPEND WSJCPP_INCLUDE_DIRS "src/core/3rdParty")
list (APPEND WSJCPP_INCLUDE_DIRS "src/3rdParty/quazip-0.7.3/quazip")
list (APPEND WSJCPP_INCLUDE_DIRS "src/utils")
list (APPEND WSJCPP_INCLUDE_DIRS "src/validators")
list (APPEND WSJCPP_INCLUDE_DIRS "src/employees")
list (APPEND WSJCPP_INCLUDE_DIRS "src/models")
list (APPEND WSJCPP_INCLUDE_DIRS "src/models/include")
list (APPEND WSJCPP_INCLUDE_DIRS "src/server")
list (APPEND WSJCPP_INCLUDE_DIRS "src/tasks")
list (APPEND WSJCPP_INCLUDE_DIRS "src/storages")
list (APPEND WSJCPP_INCLUDE_DIRS "src/storages/updates")
list (APPEND WSJCPP_INCLUDE_DIRS "src/cmd")
list (APPEND WSJCPP_INCLUDE_DIRS "src/web-server")

# core
list (APPEND WSJCPP_SOURCES "src/core/cmd_handlers.h")
list (APPEND WSJCPP_SOURCES "src/core/cmd_handlers.cpp")
list (APPEND WSJCPP_SOURCES "src/core/export_libwsjcppcli_java_android.h")
list (APPEND WSJCPP_SOURCES "src/core/export_libwsjcppcli_java_android.cpp")
list (APPEND WSJCPP_SOURCES "src/core/export_libwsjcppcli_py.h")
list (APPEND WSJCPP_SOURCES "src/core/export_libwsjcppcli_py.cpp")
list (APPEND WSJCPP_SOURCES "src/core/export_list_of_handlers.h")
list (APPEND WSJCPP_SOURCES "src/core/export_list_of_handlers.cpp")
list (APPEND WSJCPP_SOURCES "src/core/export_struct_of_storage.h")
list (APPEND WSJCPP_SOURCES "src/core/export_struct_of_storage.cpp")
list (APPEND WSJCPP_SOURCES "src/core/fallen.h")
list (APPEND WSJCPP_SOURCES "src/core/fallen.cpp")
list (APPEND WSJCPP_SOURCES "src/core/jobs_pool.h")
list (APPEND WSJCPP_SOURCES "src/core/jobs_pool.cpp")
list (APPEND WSJCPP_SOURCES "src/core/storages.h")
list (APPEND WSJCPP_SOURCES "src/core/storages.cpp")
list (APPEND WSJCPP_SOURCES "src/core/websocketserver.h")
list (APPEND WSJCPP_SOURCES "src/core/websocketserver.cpp")
list (APPEND WSJCPP_SOURCES "src/core/wsjcpp_employees.h")
list (APPEND WSJCPP_SOURCES "src/core/wsjcpp_employees.cpp")
list (APPEND WSJCPP_SOURCES "src/core/wsjcpp_export_libcli_web_js.h")
list (APPEND WSJCPP_SOURCES "src/core/wsjcpp_export_libcli_web_js.cpp")

# quazip
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/crypt.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/ioapi.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/JlCompress.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quaadler32.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quachecksum32.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quacrc32.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quagzipfile.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quaziodevice.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipdir.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipfile.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipfileinfo.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazip_global.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazip.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipnewinfo.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/unzip.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/zip.h")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/qioapi.cpp")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/JlCompress.cpp")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quaadler32.cpp")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quacrc32.cpp")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quagzipfile.cpp")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quaziodevice.cpp")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazip.cpp")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipdir.cpp")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipfile.cpp")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipfileinfo.cpp")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/quazipnewinfo.cpp")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/unzip.c")
list (APPEND WSJCPP_SOURCES "src/3rdParty/quazip-0.7.3/quazip/zip.c")

# cmd
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_classbook.h")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_classbook.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_communication.h")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_communication.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_events.h")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_events.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_games.h")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_games.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_leaks.h")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_leaks.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_lxd.h")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_lxd.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_mails.h")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_mails.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_quests_writeups.h")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_quests_writeups.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_quests.h")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_quests.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_server.h")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_server.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_support.h")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_support.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_useful_links.h")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_useful_links.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_users.h")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_users.cpp")

# models
list (APPEND WSJCPP_SOURCES "src/models/model_notification.cpp")
list (APPEND WSJCPP_SOURCES "src/models/model_notification.h")
list (APPEND WSJCPP_SOURCES "src/models/include/model_database_connection.h")
list (APPEND WSJCPP_SOURCES "src/models/sources/model_database_connection.cpp")
list (APPEND WSJCPP_SOURCES "src/models/include/model_leak.h")
list (APPEND WSJCPP_SOURCES "src/models/sources/model_leak.cpp")
list (APPEND WSJCPP_SOURCES "src/models/include/model_game.h")
list (APPEND WSJCPP_SOURCES "src/models/sources/model_game.cpp")
list (APPEND WSJCPP_SOURCES "src/models/include/model_lxd_orchestra.h")
list (APPEND WSJCPP_SOURCES "src/models/sources/model_lxd_orchestra.cpp")

# validators
list (APPEND WSJCPP_SOURCES "src/validators/validators.cpp")
list (APPEND WSJCPP_SOURCES "src/validators/validators.h")

# TODO include only if libmysqlclient exists 
list (APPEND WSJCPP_INCLUDE_DIRS "src/storages/mysql")
list (APPEND WSJCPP_SOURCES "src/storages/mysql/mysql_storage.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/mysql/mysql_storage.h")

# utils
list (APPEND WSJCPP_SOURCES "src/utils/utils_merge_text.h")
list (APPEND WSJCPP_SOURCES "src/utils/utils_merge_text.cpp")
list (APPEND WSJCPP_SOURCES "src/utils/utils_prepare_deb_package.h")
list (APPEND WSJCPP_SOURCES "src/utils/utils_prepare_deb_package.cpp")
list (APPEND WSJCPP_SOURCES "src/utils/utils_static_analizing_text.h")
list (APPEND WSJCPP_SOURCES "src/utils/utils_static_analizing_text.cpp")
list (APPEND WSJCPP_SOURCES "src/utils/utils_lxd.h")
list (APPEND WSJCPP_SOURCES "src/utils/utils_lxd.cpp")

# employees
list (APPEND WSJCPP_SOURCES "src/employees/employ_chats.h")
list (APPEND WSJCPP_SOURCES "src/employees/employ_chats.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_images.h")
list (APPEND WSJCPP_SOURCES "src/employees/employ_images.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_games.h")
list (APPEND WSJCPP_SOURCES "src/employees/employ_games.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_notify.h")
list (APPEND WSJCPP_SOURCES "src/employees/employ_notify.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_quests.h")
list (APPEND WSJCPP_SOURCES "src/employees/employ_quests.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_server_info.h")
list (APPEND WSJCPP_SOURCES "src/employees/employ_server_info.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_scoreboard.h")
list (APPEND WSJCPP_SOURCES "src/employees/employ_scoreboard.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_database.h")
list (APPEND WSJCPP_SOURCES "src/employees/employ_database.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_orchestra.h")
list (APPEND WSJCPP_SOURCES "src/employees/employ_orchestra.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_leaks.h")
list (APPEND WSJCPP_SOURCES "src/employees/employ_leaks.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_mails.h")
list (APPEND WSJCPP_SOURCES "src/employees/employ_mails.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_users.h")
list (APPEND WSJCPP_SOURCES "src/employees/employ_users.cpp")

# tasks
list (APPEND WSJCPP_SOURCES "src/tasks/runtasks.h")
list (APPEND WSJCPP_SOURCES "src/tasks/runtasks.cpp")
list (APPEND WSJCPP_SOURCES "src/tasks/mail_send_task.h")
list (APPEND WSJCPP_SOURCES "src/tasks/mail_send_task.cpp")
list (APPEND WSJCPP_SOURCES "src/tasks/notify_to_all_task.h")
list (APPEND WSJCPP_SOURCES "src/tasks/notify_to_all_task.cpp")
list (APPEND WSJCPP_SOURCES "src/tasks/add_public_events_task.h")
list (APPEND WSJCPP_SOURCES "src/tasks/add_public_events_task.cpp")
list (APPEND WSJCPP_SOURCES "src/tasks/update_max_score_game_task.h")
list (APPEND WSJCPP_SOURCES "src/tasks/update_max_score_game_task.cpp")
list (APPEND WSJCPP_SOURCES "src/tasks/update_quest_solved_task.h")
list (APPEND WSJCPP_SOURCES "src/tasks/update_quest_solved_task.cpp")
list (APPEND WSJCPP_SOURCES "src/tasks/update_user_location_task.h")
list (APPEND WSJCPP_SOURCES "src/tasks/update_user_location_task.cpp")
list (APPEND WSJCPP_SOURCES "src/tasks/update_user_rating_task.h")
list (APPEND WSJCPP_SOURCES "src/tasks/update_user_rating_task.cpp")
list (APPEND WSJCPP_SOURCES "src/tasks/lxd_async_operation_task.h")
list (APPEND WSJCPP_SOURCES "src/tasks/lxd_async_operation_task.cpp")

# updates
list (APPEND WSJCPP_SOURCES "src/storages/updates/updates_init_database.h")
list (APPEND WSJCPP_SOURCES "src/storages/updates/updates_init_database.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update0100.h")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update0100.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_u0100_mhd92m15nb.h")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_u0100_mhd92m15nb.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_mhd92m15nb_snwxenqco0.h")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_mhd92m15nb_snwxenqco0.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update0101.h")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update0101.cpp")

# web-server
list (APPEND WSJCPP_SOURCES "src/web-server/http_handler_web_user_folder.cpp")
list (APPEND WSJCPP_SOURCES "src/web-server/http_handler_web_user_folder.h")
list (APPEND WSJCPP_SOURCES "src/web-server/http_handler_web_public_folder.cpp")
list (APPEND WSJCPP_SOURCES "src/web-server/http_handler_web_public_folder.h")
list (APPEND WSJCPP_SOURCES "src/web-server/http_handler_web_admin_folder.cpp")
list (APPEND WSJCPP_SOURCES "src/web-server/http_handler_web_admin_folder.h")

# main
list (APPEND WSJCPP_SOURCES "src/main.cpp")

############################
#####
#foreach (_headerFile ${FHQSRV_HEADERS})
#    get_filename_component(_dir ${_headerFile} PATH)
#    list (APPEND WSJCPP_INCLUDE_DIRS ${_dir})
#endforeach()
#list(REMOVE_DUPLICATES WSJCPP_INCLUDE_DIRS)
#####
############################


############################
##### ZLIB
find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    list (APPEND WSJCPP_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    list (APPEND WSJCPP_LIBRARIES ${ZLIB_LIBRARIES})
endif( ZLIB_FOUND )
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

list (APPEND WSJCPP_INCLUDE_DIRS ${LIBMYSQLCLIENT_LIBRARY_DIRS})
list (APPEND WSJCPP_LIBRARIES ${LIBMYSQLCLIENT_LIBRARIES})
#####
############################

include_directories(${WSJCPP_INCLUDE_DIRS})

find_package (Qt5Core)
find_package (Qt5Sql)
find_package (Qt5Network)
find_package (Qt5WebSockets)

add_executable (fhq-server ${WSJCPP_SOURCES})

qt5_use_modules(fhq-server Core Network Sql WebSockets)
target_link_libraries(fhq-server ${WSJCPP_LIBRARIES} )

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
