cmake_minimum_required(VERSION 3.14) # For GoogleTest
project("Triangles3D")

add_subdirectory(geometry)

add_executable(triangles src/main.cc)
