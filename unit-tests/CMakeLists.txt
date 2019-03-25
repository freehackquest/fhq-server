cmake_minimum_required(VERSION 3.0)

project(unit-tests)

add_definitions(-DAPP_NAME="unit-tests")

set (CMAKE_AUTOMOC ON)
set (CMAKE_CXX_STANDARD 11)
set (EXECUTABLE_OUTPUT_PATH ${unit-tests_SOURCE_DIR})

set (UNIT_TESTS_LIBRARIES "")
set (UNIT_TESTS_INCLUDE_DIRS "")
set (UNIT_TESTS_SOURCES "")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../fhq-server/cmake/") 

# Sources

# include header dirs
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/3rdParty/nlohmann")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/3rdParty/smallsha1")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/3rdParty/md5")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/unit_tests/include")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/unit_tests") # deprecated
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/utils")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/utils/include")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/storages")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/storages/mysql")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/jobs-pool/core")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "src/unit_tests")

# json
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/3rdParty/nlohmann/json.hpp")

# small md5
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/3rdParty/md5/md5.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/3rdParty/md5/md5.cpp")

# small sha1
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/3rdParty/smallsha1/sha1.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/3rdParty/smallsha1/sha1.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/3rdParty/smallsha1/sha1_wrapper.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/3rdParty/smallsha1/sha1_wrapper.cpp")

# interfaces
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/interfaces/iserver.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/interfaces/iuser.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/interfaces/iusertoken.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/interfaces/iwebsocketserver.h")

# storages
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/storages/storage.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/storages/storage.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/storages/storages.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/storages/storages.cpp")

# jobs-pool
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/jobs-pool/core/jobs_pool.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/jobs-pool/core/jobs_pool.cpp")

# utils
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/fs.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/fs.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/logger.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/logger.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/parse_config.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/parse_config.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/str.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/str.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/ts.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/ts.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/utils_merge_text.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/utils_merge_text.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/utils.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/include/utils_levenshtein.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/sources/utils_levenshtein.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/include/utils_static_analizing_text.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/sources/utils_static_analizing_text.cpp")

# unit_tests
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_jobs_pool.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_jobs_pool.cpp")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_levenshtein.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_levenshtein.cpp")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_sha1.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_sha1.cpp")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_md5.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_md5.cpp")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_merge_text.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_merge_text.cpp")
# list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_base64.h")
# list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_base64.cpp")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_static_analizing_text.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_static_analizing_text.cpp")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_storage_escapes.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_storage_escapes.cpp")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_storage_struct.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_storage_struct.cpp")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_tests.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_tests.cpp")

# main
list (APPEND UNIT_TESTS_SOURCES "src/main.cpp")


############################
##### ZLIB
find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    list (APPEND UNIT_TESTS_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    list (APPEND UNIT_TESTS_LIBRARIES ${ZLIB_LIBRARIES})
endif( ZLIB_FOUND )
#####
############################


############################
##### LIBPNG
find_package(PNG REQUIRED)
if (NOT PNG_FOUND)
    message(FATAL_ERROR "You don't seem to have libpng development libraries installed (apt isntall libpng-dev)")
else ()
    list (APPEND UNIT_TESTS_INCLUDE_DIRS ${PNG_INCLUDE_DIR})
    list (APPEND UNIT_TESTS_LIBRARIES ${PNG_LIBRARY})
endif ()
#####
############################

############################
##### CURL
FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  list (APPEND UNIT_TESTS_INCLUDE_DIRS ${CURL_INCLUDE_DIR})
  list (APPEND UNIT_TESTS_LIBRARIES ${CURL_LIBRARIES})
ELSE(CURL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
ENDIF(CURL_FOUND)
#####
############################


############################
##### mysql 
FIND_PACKAGE(MYSQL)
IF(MYSQL_FOUND)
  list (APPEND UNIT_TESTS_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR})
  list (APPEND UNIT_TESTS_LIBRARIES ${MYSQL_LIBRARY})
  list (APPEND UNIT_TESTS_INCLUDE_DIRS "src/storages/mysql")
  list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/storages/mysql/mysql_storage.cpp")
  list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/storages/mysql/mysql_storage.h")
ELSE(MYSQL_FOUND)
  MESSAGE(WARNING "Could not find library for MySQL. If not need just ignore this message.")
ENDIF(MYSQL_FOUND)
#####
############################


############################
##### PostgreSQL

# FIND_PACKAGE(PostgreSQL)
# IF(PostgreSQL_FOUND)
#  list (APPEND UNIT_TESTS_INCLUDE_DIRS ${PostgreSQL_INCLUDE_DIR})
#   list (APPEND UNIT_TESTS_LIBRARIES ${PostgreSQL_LIBRARIES})
#   list (APPEND UNIT_TESTS_INCLUDE_DIRS "src/storages/postgresql")
#   list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/storages/postgresql/postgresql_storage.cpp")
#   list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/storages/postgresql/postgresql_storage.h")
# ELSE(PostgreSQL_FOUND)
#   MESSAGE(WARNING "Could not find library for PostgreSQL. If it's not need for you just ignore this message.")
# ENDIF(PostgreSQL_FOUND)
#####
############################

include_directories(${UNIT_TESTS_INCLUDE_DIRS})
add_executable (unit-tests ${UNIT_TESTS_SOURCES})

target_link_libraries(unit-tests -lpthread ${UNIT_TESTS_LIBRARIES} )

install(
    TARGETS
        unit-tests
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_PREFIX}/bin
)
