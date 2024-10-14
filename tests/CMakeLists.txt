cmake_minimum_required(VERSION 3.14) # For GoogleTest
project("Triangles3D")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        "origin/main"
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(geometry_tests geometry_tests.cc)
target_link_libraries(geometry_tests geometry;gtest_main)
