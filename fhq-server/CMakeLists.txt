cmake_minimum_required(VERSION 3.0)

project(fhq-server)

set(CMAKE_AUTOMOC ON)
set(EXECUTABLE_OUTPUT_PATH ${fhq-server_SOURCE_DIR})

add_definitions(-DVERSION_STRING="0.2.0")

file(GLOB_RECURSE FHQSRV_SOURCES "src/*.cpp")
file(GLOB_RECURSE FHQSRV_HEADERS "src/*.h")

set (FHQSRV_INCLUDE_DIRS "")
foreach (_headerFile ${FHQSRV_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND FHQSRV_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES FHQSRV_INCLUDE_DIRS)

find_package (Qt5Core)
find_package (Qt5Sql)
find_package (Qt5Network)
find_package (Qt5WebSockets)

add_executable (fhq-server ${FHQSRV_SOURCES})

qt5_use_modules(fhq-server Core Network Sql)

# target_link_libraries(fhq-server Qt5::Core)

install(
    TARGETS
        fhq-server
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_PREFIX}/bin
)
