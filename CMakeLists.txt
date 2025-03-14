cmake_minimum_required(VERSION 3.14) # For GoogleTest
project("Triangles3D")

find_package(spdlog REQUIRED)

add_subdirectory(geometry)
add_subdirectory(driver)

option(ENABLE_TESTS "Enable testing" ON)
if (ENABLE_TESTS)
  find_package(GTest REQUIRED)
  find_package(CGAL REQUIRED)
  include(GoogleTest)
  enable_testing()
  add_subdirectory(tests)
endif()
