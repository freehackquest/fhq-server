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

set (CMAKE_CXX_STANDARD 11)

set(EXE_NAME tests)

if (MSVC AND USE_STATIC_CRT)
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
endif()

if (NOT EASYZIP_BUILD_SHARED_LIBS)
	# Use ppx static library
	add_definitions(-DEASYZIP_STATIC)
endif()

file(GLOB SOURCE_FILES 			./*.cpp ./*.h)

add_executable(
	${EXE_NAME}
	${SOURCE_FILES}
	)
	
# Win32 Console
if (WIN32 OR _WIN32)
	set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
	set_target_properties(${EXE_NAME} PROPERTIES COMPILE_DEFINITIONS "_CONSOLE")
endif()

# zlib
find_package(ZLIB REQUIRED)
target_link_libraries(${EXE_NAME} ZLIB::ZLIB)

find_package(GTest MODULE REQUIRED)
target_link_libraries(${EXE_NAME} GTest::GTest GTest::Main)

add_test(NAME unit
COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests)

if(EASYZIP_BUILD_SHARED_LIBS)
	add_dependencies(${EXE_NAME} easyzip)
	
	target_link_libraries(${EXE_NAME} 
		$<TARGET_LINKER_FILE:easyzip> )
else()
	add_dependencies(${EXE_NAME} easyzip-static)
	
	target_link_libraries(${EXE_NAME} 
		$<TARGET_LINKER_FILE:easyzip-static> )

endif()

