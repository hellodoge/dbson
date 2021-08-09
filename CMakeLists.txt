cmake_minimum_required(VERSION 3.19)
project(dbson)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS "-fconcepts")

find_package(Boost 1.76.0 REQUIRED)

include_directories(include)
include_directories(${Boost_INCLUDE_DIR})

file(GLOB_RECURSE dbson_source src/*.cpp)
file(GLOB_RECURSE dbson_include include/*.hpp)

add_executable(dbson ${dbson_source} ${dbson_include})

target_link_libraries(dbson ${Boost_LIBRARIES} pthread)