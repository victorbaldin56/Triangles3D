cmake_minimum_required(VERSION 3.14) # For GoogleTest

if (${BUILD_TEST_GENERATOR})
  find_package(CGAL REQUIRED) # for end-to-end test generator
  add_executable(generator generator.cc)
  target_link_libraries(generator CGAL::CGAL)
endif()

add_test(NAME e2eTest
         COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/run_tests.py)
