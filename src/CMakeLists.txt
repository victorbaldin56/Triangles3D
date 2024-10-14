cmake_minimum_required(VERSION 3.14) # For GoogleTest
project("Triangles3D")

add_executable(triangles main.cc)
target_link_libraries(triangles geometry)
