add_library(triangles_gl INTERFACE)
target_include_directories(triangles_gl INTERFACE include)
target_link_libraries(triangles_gl INTERFACE ImGui-SFML::ImGui-SFML)
target_link_libraries(triangles_gl INTERFACE glhpp)
