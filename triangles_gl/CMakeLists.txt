add_library(triangles_gl src/window.cc)
target_include_directories(triangles_gl PUBLIC include)
target_link_libraries(triangles_gl sfml::sfml)
