# Got from https://github.com/snikulov/sqlite.cmake.build/blob/master/CMakeLists.txt
#--------------------------------------------
# SQLite build script for amalgamation
#
# default folder structure as follows
# <project root folder>
#      src/
#--------------------------------------------
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(PROJECT_NAME sqlite)

# define project for C language
project(${PROJECT_NAME} C)

message("Building on: ${CMAKE_SYSTEM_NAME}")

option(SQLITE_BUILD_STATIC  "Build SQLite static library" ON)
option(SQLITE_BUILD_SHARED  "Build SQLite shared library" OFF)
option(WITH_SQLITE_DEBUG    "Build SQLite debug features" OFF)
option(WITH_SQLITE_MEMDEBUG "Build SQLite memory debug features" OFF)
option(WITH_SQLITE_RTREE    "Build R*Tree index extension" OFF)

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")

set(SQLITE_WIN_BUILD OFF)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(SQLITE_WIN_BUILD ON)
endif()

set(SQLITE_WINRT_BUILD OFF)
if(${CMAKE_SYSTEM_NAME} MATCHES "WindowsStore")
    set(SQLITE_WINRT_BUILD ON)
endif()

# ---------------------
# Version detection
# ---------------------
file(STRINGS "${PROJECT_SOURCE_DIR}/src/sqlite3.h" _sqlite_api_h_VER_STRING REGEX ".*#define[ ]+SQLITE_VERSION[ ]+")

string(REGEX MATCH "[0-9\\.]+" SQLITE_VER_STRING ${_sqlite_api_h_VER_STRING})
string(REGEX MATCHALL "[0-9]+" _sqlite_ver_LIST "${SQLITE_VER_STRING}")
list(LENGTH _sqlite_ver_LIST _sqlite_list_len)
list(GET _sqlite_ver_LIST 0 SQLITE_VER_MAJOR)
list(GET _sqlite_ver_LIST 1 SQLITE_VER_MINOR)
list(GET _sqlite_ver_LIST 2 SQLITE_VER_PATCH)
if(_sqlite_list_len EQUAL 4)
    list(GET _sqlite_ver_LIST 3 SQLITE_VER_PATCHLEVEL)
    message("Patch level: ${SQLITE_VER_PATCHLEVEL}")
endif()

message("FOUND: SQLite version = ${SQLITE_VER_STRING}")

set(SQLITE_DYN_NAME "${PROJECT_NAME}${SQLITE_VER_MAJOR}")
set(SQLITE_STATIC_NAME "${SQLITE_DYN_NAME}-static")

find_package(Threads REQUIRED)

# add include path for project
include_directories(${PROJECT_SOURCE_DIR}/src)

set(SRC_LIB_FILE ${PROJECT_SOURCE_DIR}/src/sqlite${SQLITE_VER_MAJOR}.c)
set(SRC_SHELL_FILE ${PROJECT_SOURCE_DIR}/src/shell.c)
file(GLOB HDR_FILES ${PROJECT_SOURCE_DIR}/src/*.h)

# adding compilation flags depending on options
if(WITH_SQLITE_DEBUG)
    add_definitions(-DSQLITE_DEBUG)
endif()
if(WITH_SQLITE_MEMDEBUG)
    add_definitions(-DSQLITE_MEMDEBUG)
endif()
if(WITH_SQLITE_RTREE)
    add_definitions(-DSQLITE_ENABLE_RTREE)
endif()

if(SQLITE_WINRT_BUILD)
    # Don't use tools that are unavailable on RT platforms
    add_definitions(-DSQLITE_OS_WINRT)
endif()

if (SQLITE_BUILD_STATIC)
    # build static library
    add_library(${SQLITE_STATIC_NAME} STATIC ${SRC_LIB_FILE})
endif()

if (SQLITE_BUILD_SHARED)
    # build dynamic library
    add_library(${SQLITE_DYN_NAME} SHARED ${SRC_LIB_FILE})
    if (SQLITE_WIN_BUILD)
        #then we do dll library, so need to export api
        set_target_properties(${SQLITE_DYN_NAME} PROPERTIES DEFINE_SYMBOL "SQLITE_API=__declspec(dllexport)")
    endif()
endif()

# build shell executable
# add_executable(shell ${SRC_SHELL_FILE})

# if (SQLITE_BUILD_SHARED)
#     # preferred is shared library
#     target_link_libraries(shell ${SQLITE_DYN_NAME} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})
# elseif(SQLITE_BUILD_STATIC)
#     target_link_libraries(shell ${SQLITE_STATIC_NAME} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})
# else()
#     # no static or dynamic option selected
#     message(FATAL_ERROR "either static or dynamic option should be selected")
# endif()

# # installation
# list(APPEND TO_INSTALL shell)
# if(SQLITE_BUILD_SHARED)
#     list(APPEND TO_INSTALL ${SQLITE_DYN_NAME})
# endif()
# if(SQLITE_BUILD_STATIC)
#     list(APPEND TO_INSTALL ${SQLITE_STATIC_NAME})
# endif()

# install(TARGETS ${TO_INSTALL}
#         RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
#         ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
#         LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
# install(FILES ${HDR_FILES} DESTINATION "${INSTALL_INC_DIR}")