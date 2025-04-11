add_library(triangles_gl INTERFACE)
target_include_directories(triangles_gl INTERFACE include)
target_link_libraries(triangles_gl INTERFACE sfml::sfml)
target_link_libraries(triangles_gl INTERFACE glhpp)
