add_definitions(-DWSJCPP_VERSION="v0.0.1")
add_definitions(-DWSJCPP_NAME="fhq-server")

set(CMAKE_CXX_STANDARD 11)

set (WSJCPP_LIBRARIES "")
set (WSJCPP_INCLUDE_DIRS "")
set (WSJCPP_SOURCES "")

# wsjcpp-hashes
list (APPEND WSJCPP_INCLUDE_DIRS "src.wsjcpp/wsjcpp-hashes/")
list (APPEND WSJCPP_SOURCES "src.wsjcpp/wsjcpp-hashes/smallsha1.h")
list (APPEND WSJCPP_SOURCES "src.wsjcpp/wsjcpp-hashes/smallsha1.cpp")
list (APPEND WSJCPP_SOURCES "src.wsjcpp/wsjcpp-hashes/md5.h")
list (APPEND WSJCPP_SOURCES "src.wsjcpp/wsjcpp-hashes/md5.cpp")
list (APPEND WSJCPP_SOURCES "src.wsjcpp/wsjcpp-hashes/wsjcpp_hashes.h")
list (APPEND WSJCPP_SOURCES "src.wsjcpp/wsjcpp-hashes/wsjcpp_hashes.cpp")
