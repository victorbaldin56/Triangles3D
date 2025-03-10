cmake_minimum_required(VERSION 3.14) # For GoogleTest

add_executable(triangles main.cc)
target_link_libraries(triangles geometry)
