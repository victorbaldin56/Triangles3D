add_executable(triangles src/main.cc src/cmd_parser.cc)
target_link_libraries(triangles geometry)
target_link_libraries(triangles Boost::program_options)
target_include_directories(triangles PRIVATE include)
