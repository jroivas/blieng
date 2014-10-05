cmake_minimum_required(VERSION 2.8.8)

project(blieng)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_REENTRANT")

find_package(Qt5Core REQUIRED)

add_subdirectory(blieng)

enable_testing()
add_test(blieng_unittest
    blieng/tests/blieng_test)
