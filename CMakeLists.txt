cmake_minimum_required(VERSION 3.14) # For GoogleTest
project("Triangles3D")

add_subdirectory(geometry)
add_subdirectory(src)

if (${BUILD_TESTING})
  find_package(GTest REQUIRED)
  include(GoogleTest)
  enable_testing()
  add_subdirectory(tests)
endif()
