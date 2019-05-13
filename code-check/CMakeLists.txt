cmake_minimum_required(VERSION 3.0)

project(code-check)

add_definitions(-DAPP_NAME="code-check")

set (CMAKE_AUTOMOC ON)
set (CMAKE_CXX_STANDARD 11)
set (EXECUTABLE_OUTPUT_PATH ${code-check_SOURCE_DIR})

set (CODE_CHECK_LIBRARIES "")
set (CODE_CHECK_INCLUDE_DIRS "")
set (CODE_CHECK_SOURCES "")

# Sources

# include header dirs
list (APPEND CODE_CHECK_INCLUDE_DIRS "src")

list (APPEND CODE_CHECK_SOURCES "src/config.h")
list (APPEND CODE_CHECK_SOURCES "src/config.cpp")
list (APPEND CODE_CHECK_SOURCES "src/helpers.h")
list (APPEND CODE_CHECK_SOURCES "src/helpers.cpp")
list (APPEND CODE_CHECK_SOURCES "src/json.hpp")
list (APPEND CODE_CHECK_SOURCES "src/line_checkers.h")
list (APPEND CODE_CHECK_SOURCES "src/line_checkers.cpp")
list (APPEND CODE_CHECK_SOURCES "src/logger.h")
list (APPEND CODE_CHECK_SOURCES "src/logger.cpp")
list (APPEND CODE_CHECK_SOURCES "src/main.cpp")

include_directories(${CODE_CHECK_INCLUDE_DIRS})
add_executable (code-check ${CODE_CHECK_SOURCES})

target_link_libraries(code-check -lpthread ${CODE_CHECK_LIBRARIES} )

install(
    TARGETS
        code-check
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_PREFIX}/bin
)
