#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

#include "GL/glew.h"

#include "detail/handler.hh"

namespace glhpp {

class Shader final {
 public:
  Shader(const std::filesystem::path& path, GLenum type)
      : code_(loadFile(path)), type_(type) {
    id_ = GLHPP_DETAIL_ERROR_HANDLER(glCreateShader, type_);
  }

 private:
  static std::string loadFile(const std::filesystem::path& path) {
    std::string code;
    std::ifstream shader_file;
    shader_file.open(path, std::ios::in | std::ios::binary);
    if (!shader_file.is_open()) {
      auto what = "Failed to open file " + path.string();
      throw std::runtime_error(what);
    }
    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf();
    shader_file.close();
    code = shader_stream.str();
    return code;
  }

 private:
  std::string code_;
  GLenum type_;
  GLuint id_;
};

}
