##################################################################################
#    __ _
#   / _| |__   __ _       ___  ___ _ ____   _____ _ __
#  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
#  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
#  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
#                |_|
#
# Copyright (c) 2011-2025 FreeHackQuest <freehackquest@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
##################################################################################
# fhq-server CMake script

cmake_minimum_required(VERSION 3.0)

project(fhq-server)

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

#### BEGIN_WSJCPP_APPEND

#### END_WSJCPP_APPEND

set (CMAKE_AUTOMOC ON)
set (CMAKE_CXX_STANDARD 17)
set (EXECUTABLE_OUTPUT_PATH ${fhq-server_SOURCE_DIR})
set (QTCORE_LIBRARIES ${Qt5Core_LIBRARIES})
include_directories(${Qt5Core_INCLUDE_DIRS})

# Sources

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")
list (APPEND WSJCPP_INCLUDE_DIRS "src/argument_processors")
list (APPEND WSJCPP_INCLUDE_DIRS "src/core")
list (APPEND WSJCPP_INCLUDE_DIRS "src/database")
list (APPEND WSJCPP_INCLUDE_DIRS "src/core/3rdParty")
list (APPEND WSJCPP_INCLUDE_DIRS "src/utils")
list (APPEND WSJCPP_INCLUDE_DIRS "src/validators")
list (APPEND WSJCPP_INCLUDE_DIRS "src/employees")
list (APPEND WSJCPP_INCLUDE_DIRS "src/models")
list (APPEND WSJCPP_INCLUDE_DIRS "src/server")
list (APPEND WSJCPP_INCLUDE_DIRS "src/tasks")
list (APPEND WSJCPP_INCLUDE_DIRS "src/storages")
list (APPEND WSJCPP_INCLUDE_DIRS "src/storages/updates")
list (APPEND WSJCPP_INCLUDE_DIRS "src/cmd")
list (APPEND WSJCPP_INCLUDE_DIRS "src/web-server")

# argument_processor
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_main.cpp")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_api.cpp")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_config.cpp")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_database.cpp")

# core
list (APPEND WSJCPP_SOURCES "src/core/cmd_handlers.h")
list (APPEND WSJCPP_SOURCES "src/core/cmd_handlers.cpp")
list (APPEND WSJCPP_SOURCES "src/core/export_libwsjcppcli_java_android.h")
list (APPEND WSJCPP_SOURCES "src/core/export_libwsjcppcli_java_android.cpp")
list (APPEND WSJCPP_SOURCES "src/core/export_libwsjcppcli_py.h")
list (APPEND WSJCPP_SOURCES "src/core/export_libwsjcppcli_py.cpp")
list (APPEND WSJCPP_SOURCES "src/core/jobs_pool.h")
list (APPEND WSJCPP_SOURCES "src/core/jobs_pool.cpp")
list (APPEND WSJCPP_SOURCES "src/core/websocketserver.h")
list (APPEND WSJCPP_SOURCES "src/core/websocketserver.cpp")
list (APPEND WSJCPP_SOURCES "src/core/employees.h")
list (APPEND WSJCPP_SOURCES "src/core/employees.cpp")
list (APPEND WSJCPP_SOURCES "src/core/wsjcpp_export_libcli_web_js.h")
list (APPEND WSJCPP_SOURCES "src/core/wsjcpp_export_libcli_web_js.cpp")

# database
list (APPEND WSJCPP_SOURCES "src/database/fhq_server_database_file.cpp")
list (APPEND WSJCPP_SOURCES "src/database/fhq_server_db_email_delivery.cpp")
list (APPEND WSJCPP_SOURCES "src/database/fhq_server_db_useful_links.cpp")
list (APPEND WSJCPP_SOURCES "src/database/fhq_server_db_uuids.cpp")

# cmd
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_classbook.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_communication.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_events.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_games.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_leaks.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_lxd.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_mails.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_quests_writeups.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_quests.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_server.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_support.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_useful_links.cpp")
list (APPEND WSJCPP_SOURCES "src/cmd/cmd_handlers_users.cpp")

# models
list (APPEND WSJCPP_SOURCES "src/models/model_game.cpp")
list (APPEND WSJCPP_SOURCES "src/models/model_notification.cpp")
list (APPEND WSJCPP_SOURCES "src/models/model_quest_file.cpp")
list (APPEND WSJCPP_SOURCES "src/models/model_database_connection.cpp")
list (APPEND WSJCPP_SOURCES "src/models/model_leak.cpp")
list (APPEND WSJCPP_SOURCES "src/models/model_lxd_orchestra.cpp")
list (APPEND WSJCPP_SOURCES "src/models/model_useful_link.cpp")
list (APPEND WSJCPP_SOURCES "src/models/model_useful_link_comment.cpp")
list (APPEND WSJCPP_SOURCES "src/models/model_useful_link_tag.cpp")
list (APPEND WSJCPP_SOURCES "src/models/model_useful_link_user_favorite.cpp")

# validators
list (APPEND WSJCPP_SOURCES "src/validators/validators.cpp")
list (APPEND WSJCPP_SOURCES "src/validators/validators.h")

# TODO include only if libmysqlclient exists
list (APPEND WSJCPP_INCLUDE_DIRS "src/storages/mysql")
list (APPEND WSJCPP_SOURCES "src/storages/mysql/mysql_storage.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/mysql/mysql_storage.h")

# utils
list (APPEND WSJCPP_SOURCES "src/utils/utils_prepare_deb_package.h")
list (APPEND WSJCPP_SOURCES "src/utils/utils_prepare_deb_package.cpp")
list (APPEND WSJCPP_SOURCES "src/utils/utils_lxd.h")
list (APPEND WSJCPP_SOURCES "src/utils/utils_lxd.cpp")

# employees
list (APPEND WSJCPP_SOURCES "src/employees/employ_chats.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_images.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_games.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_files.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_notify.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_quests.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_server_info.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_scoreboard.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_database.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_orchestra.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_leaks.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_mails.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_users.cpp")
list (APPEND WSJCPP_SOURCES "src/employees/employ_uuids.cpp")

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
list (APPEND WSJCPP_SOURCES "src/storages/updates/updates_init_database.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update0100.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_u0100_mhd92m15nb.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_mhd92m15nb_snwxenqco0.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_snwxenqco0_csww6lwcrp.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_csww6lwcrp_722kt134lq.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_722kt134lq_6c8d4zm6u2.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_6c8d4zm6u2_pmlyt8k7nm.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_pmlyt8k7nm_215438peu8.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_215438peu8_9gwc2tm5ah.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_9gwc2tm5ah_on73sheet9.cpp")
list (APPEND WSJCPP_SOURCES "src/storages/updates/update_on73sheet9_ili7looowo.cpp")
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
#foreach (_headerFile ${WSJCPP_HEADERS})
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

# easyzip
set(EASYZIP_BUILD_SHARED_LIBS OFF CACHE BOOL "")
add_subdirectory(src/third-party/easyzip)
list (APPEND WSJCPP_LIBRARIES easyzip-static)
list (APPEND WSJCPP_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/third-party/easyzip/include/easyzip")
message(STATUS "Easyzip Includes: ${PROJECT_SOURCE_DIR}/src/third-party/easyzip/include/easyzip")

# libhv
set(WITH_OPENSSL OFF CACHE BOOL "")
set(WITH_CURL OFF CACHE BOOL "")
# set(WITH_MQTT OFF CACHE BOOL "")
# set(WITH_EVPP OFF CACHE BOOL "")
set(BUILD_EXAMPLES OFF CACHE BOOL "")
# set(WITH_HTTP_CLIENT OFF CACHE BOOL "")
set(BUILD_SHARED OFF CACHE BOOL "")

add_subdirectory(src/third-party/libhv)
list (APPEND WSJCPP_LIBRARIES hv_static)
list (APPEND WSJCPP_INCLUDE_DIRS "${PROJECT_BINARY_DIR}/src/third-party/libhv/include/hv")


add_subdirectory(src/third-party/sqlite3)
# https://www.sqlite.org/threadsafe.html
# target_compile_definitions(sqlite3-static
#   PRIVATE
#     SQLITE_THREADSAFE=0
# )
list (APPEND WSJCPP_LIBRARIES sqlite3-static)
message(STATUS "Sqlite Includes: ${PROJECT_SOURCE_DIR}/src/third-party/sqlite3/src")
list (APPEND WSJCPP_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/third-party/sqlite3/src")

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


