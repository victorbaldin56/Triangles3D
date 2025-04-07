add_library(geometry INTERFACE)
target_link_libraries(geometry INTERFACE spdlog::spdlog)
target_include_directories(geometry INTERFACE include)
target_compile_features(geometry INTERFACE cxx_std_17)
