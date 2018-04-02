cmake_minimum_required(VERSION 3.0)

project(fhq-server)

set(CMAKE_AUTOMOC ON)
set(EXECUTABLE_OUTPUT_PATH ${fhq-server_SOURCE_DIR})

add_definitions(-DVERSION_STRING="0.2.0")


include_directories("src/3rdParty/nlohmann")
include_directories("src/3rdParty/quazip-0.7.3/quazip")
include_directories("src/3rdParty/smtp")
include_directories("src/cache" "src/cache/include")
include_directories("src/cmd")
include_directories("src/cmd/classbook/include")
include_directories("src/cmd/events/include")
include_directories("src/cmd/games/include")
include_directories("src/cmd/mails/include")
include_directories("src/cmd/quests/include")
include_directories("src/cmd/server/include")
include_directories("src/cmd/support/include")
include_directories("src/cmd/users/include")
include_directories("src/employees" "src/employees/include")
include_directories("src/interfaces")
include_directories("src/models" "src/models/include")
include_directories("src/server/include")
include_directories("src/tasks" "src/tasks/include")
include_directories("src/unit_tests" "src/unit_tests/include")
include_directories("src/updates" "src/updates/include")
include_directories("src/utils" "src/utils/include")

file(GLOB_RECURSE FHQSRV_SOURCES "src/*.cpp")
file(GLOB_RECURSE FHQSRV_HEADERS "src/*.h")

set (FHQSRV_INCLUDE_DIRS "")
foreach (_headerFile ${FHQSRV_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND FHQSRV_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES FHQSRV_INCLUDE_DIRS)

find_package (Qt5Core)
find_package (Qt5Gui)
find_package (Qt5Sql)
find_package (Qt5Network)
find_package (Qt5WebSockets)

add_executable (fhq-server ${FHQSRV_SOURCES})

qt5_use_modules(fhq-server Core Network Sql WebSockets Gui)

# zlib find

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries(fhq-server ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

# target_link_libraries(fhq-server Qt5::Core)

install(
    TARGETS
        fhq-server
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_PREFIX}/bin
)
