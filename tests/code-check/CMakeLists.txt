cmake_minimum_required(VERSION 3.0)

project(code-check)

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

set (CMAKE_CXX_STANDARD 11)
set (EXECUTABLE_OUTPUT_PATH ${code-check_SOURCE_DIR})

# Sources

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")

list (APPEND WSJCPP_SOURCES "src/config.h")
list (APPEND WSJCPP_SOURCES "src/config.cpp")
list (APPEND WSJCPP_SOURCES "src/line_checkers.h")
list (APPEND WSJCPP_SOURCES "src/line_checkers.cpp")
list (APPEND WSJCPP_SOURCES "src/main.cpp")

include_directories(${WSJCPP_INCLUDE_DIRS})
add_executable (code-check ${WSJCPP_SOURCES})

target_link_libraries(code-check -lpthread ${WSJCPP_LIBRARIES} )

install(
    TARGETS
        code-check
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_PREFIX}/bin
)
