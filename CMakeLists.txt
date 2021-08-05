cmake_minimum_required(VERSION 3.19)
project(dbson)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS "-fconcepts")

find_package(Boost 1.65.0 REQUIRED)

include_directories(include)

add_executable(dbson main.cpp src/binary_json/binary_json.cpp include/binary_json/binary_json.hpp src/services/db_service.cpp include/services/db_service.hpp include/binary_json/object.hpp include/binary_json/serialize.hpp)
target_link_libraries(dbson ${Boost_LIBRARIES})