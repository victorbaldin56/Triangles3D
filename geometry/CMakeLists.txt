cmake_minimum_required(VERSION 3.14) # For GoogleTest
project("geometry")

add_library(geometry INTERFACE)
target_include_directories(geometry INTERFACE .)
target_compile_features(geometry INTERFACE cxx_std_14)
