##################################################################################
#                      Project
#    __ _
#   / _| |__   __ _       ___  ___ _ ____   _____ _ __
#  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
#  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
#  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
#                |_|
#
# MIT License
#
# Copyright (c) 2011-2026 FreeHackQuest <freehackquest@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Original repository: https://github.com/freehackquest/fhq-server
#
##################################################################################

file(GLOB ALL_TESTS
     "test_*.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY tests)

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../third-party/sqlite3 ${CMAKE_CURRENT_BINARY_DIR}/sqlite3)
list (APPEND WSJCPP_LIBRARIES sqlite3-static)
list (APPEND TEST_FHQ_SERVER_INCLUDE_DIRS "${PROJECT_BINARY_DIR}/../third-party/sqlite3/src")

# wsjcpp-core
list (APPEND TEST_FHQ_SERVER_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/../../src.wsjcpp/wsjcpp_core/")
list (APPEND TEST_FHQ_SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../src.wsjcpp/wsjcpp_core/wsjcpp_core.cpp")

# nlohmann/json
list (APPEND TEST_FHQ_SERVER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../../src.wsjcpp/nlohmann_json/")

# wsjcpp-employees
list (APPEND TEST_FHQ_SERVER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../../src.wsjcpp/wsjcpp_employees/")
list (APPEND TEST_FHQ_SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../src.wsjcpp/wsjcpp_employees/wsjcpp_employees.cpp")

# wsjcpp_validators
list (APPEND TEST_FHQ_SERVER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../../src.wsjcpp/wsjcpp_validators/")
list (APPEND TEST_FHQ_SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../src.wsjcpp/wsjcpp_validators/wsjcpp_validators.cpp")

# wsjcpp-yaml
list (APPEND TEST_FHQ_SERVER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../../src.wsjcpp/wsjcpp_yaml/")
list (APPEND TEST_FHQ_SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../src.wsjcpp/wsjcpp_yaml/wsjcpp_yaml.cpp")

# wsjcpp-storages
list (APPEND TEST_FHQ_SERVER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../../src.wsjcpp/wsjcpp_storages/")
list (APPEND TEST_FHQ_SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../src.wsjcpp/wsjcpp_storages/wsjcpp_storages.cpp")

# fhq-server
list (APPEND TEST_FHQ_SERVER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../")
list (APPEND TEST_FHQ_SERVER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../storages/")
list (APPEND TEST_FHQ_SERVER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../storages/mysql/")
list (APPEND TEST_FHQ_SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../storages/mysql/mysql_storage.cpp")
list (APPEND TEST_FHQ_SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../validators/validators.cpp")
list (APPEND TEST_FHQ_SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../core/jobs_pool.cpp")


foreach(_TEST ${ALL_TESTS})
    get_filename_component(TEST_NAME ${_TEST} NAME_WE)
    add_executable(${TEST_NAME}
        ${_TEST}
        ${TEST_FHQ_SERVER_SOURCES}
    )
    target_link_libraries(${TEST_NAME} -lpthread ${WSJCPP_LIBRARIES})

    add_test(
      NAME ${TEST_NAME}
      COMMAND $<TARGET_FILE:${TEST_NAME}>
    )
    message(${CMAKE_CURRENT_BINARY_DIR}/tests)
    set_target_properties (${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY tests)
endforeach()
