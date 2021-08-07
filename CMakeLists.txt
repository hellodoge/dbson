cmake_minimum_required(VERSION 3.19)
project(dbson)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS "-fconcepts")

find_package(Boost 1.76.0 REQUIRED)

include_directories(include)

add_executable(dbson main.cpp src/services/db_service.cpp include/services/db_service.hpp include/binary_json/object.hpp include/binary_json/serialize.hpp include/binary_json/deserialize.hpp include/exceptions/eof_error.hpp include/util/bswap.hpp include/db/collection.hpp src/db/collection.cpp include/db/object.hpp src/db/object.cpp src/db/object.hpp)
target_link_libraries(dbson ${Boost_LIBRARIES})