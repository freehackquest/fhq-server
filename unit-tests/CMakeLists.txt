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
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/core")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/core/3rdParty")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/utils")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/validators")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/utils")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/storages")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "../fhq-server/src/storages/mysql")
list (APPEND UNIT_TESTS_INCLUDE_DIRS "src/unit_tests")

# core - 3rdParty
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/3rdParty/json.hpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/3rdParty/md5.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/3rdParty/md5.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/3rdParty/smallsha1.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/3rdParty/smallsha1.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/3rdParty/sha1_wrapper.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/3rdParty/sha1_wrapper.cpp")

# core
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/fallen.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/fallen.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/jobs_pool.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/jobs_pool.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/storages.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/storages.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/validators_base.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/core/validators_base.h")

# validators
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/validators/validators.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/validators/validators.h")

# utils
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/utils_merge_text.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/utils_merge_text.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/utils_levenshtein.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/utils_levenshtein.cpp")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/utils_static_analizing_text.h")
list (APPEND UNIT_TESTS_SOURCES "../fhq-server/src/utils/utils_static_analizing_text.cpp")

# unit_tests

list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_fallen.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_fallen.cpp")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_hashes.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_hashes.cpp")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_jobs_pool.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_jobs_pool.cpp")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_levenshtein.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_levenshtein.cpp")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_merge_text.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_merge_text.cpp")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_storage_updates.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_storage_updates.cpp")
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
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_validators.h")
list (APPEND UNIT_TESTS_SOURCES "src/unit_tests/unit_test_validators.cpp")

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
