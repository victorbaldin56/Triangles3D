cmake_minimum_required(VERSION 3.14) # For GoogleTest

add_executable(geometry_tests geometry_tests.cc)
target_link_libraries(geometry_tests geometry;GTest::gtest_main)

gtest_discover_tests(geometry_tests)
