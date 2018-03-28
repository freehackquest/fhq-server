project(fhq-server)

cmake_minimum_required(VERSION 2.6)

set(EXECUTABLE_OUTPUT_PATH ${fhq-server_SOURCE_DIR})

add_executable(fhq-server main.cpp)

install(
    TARGETS
        fhq-server
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_PREFIX}/bin
)
