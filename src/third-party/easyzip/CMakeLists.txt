###############################################################################
# Copyright (C) 2018 - 2020, winsoft666, <winsoft666@outlook.com>.
#
# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
# EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
#
# Expect bugs
#
# Please use and enjoy. Please let me know of any bugs/improvements
# that you have found/implemented and I will fix/incorporate them into this
# file.
###############################################################################

cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

project(easyzip C CXX)

option(EASYZIP_BUILD_SHARED_LIBS "Build shared libraries" ON)
option(EASYZIP_BUILD_TESTS "Build tests project" OFF)
option(EASYZIP_USE_STATIC_CRT "Set to ON to build with static CRT on Windows (/MT)." OFF)

set(DEBUG_LIBNAME_SUFFIX "-d" 
	CACHE STRING "Optional suffix to append to the library name for a debug build")
mark_as_advanced(DEBUG_LIBNAME_SUFFIX)

# Unicode Support
add_definitions(-DUNICODE -D_UNICODE -DNOMINMAX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(./include)

add_subdirectory(src)

if(EASYZIP_BUILD_TESTS)
	add_subdirectory(tests)
	enable_testing()
endif()