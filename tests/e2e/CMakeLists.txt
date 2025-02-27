cmake_minimum_required(VERSION 3.14) # For GoogleTest

add_executable(generator generator.cc)
target_link_libraries(generator CGAL::CGAL)
