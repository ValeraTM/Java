cmake_minimum_required(VERSION 2.6)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -std=c++14 -Wall -fprofile-arcs -ftest-coverage")

# Link runTests with what we want to test and the GTest and pthread library
add_executable(test test.cpp)
target_link_libraries(test ${GTEST_LIBRARIES} pthread)