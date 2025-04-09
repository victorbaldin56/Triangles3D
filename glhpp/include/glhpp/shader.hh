#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <vector>

#include "GL/glew.h"
#include "detail/error_handler.hh"

namespace glhpp {

class Shader final {
  struct ShaderDeleter {
    auto operator()(GLuint* shader_ptr) const noexcept {
      glDeleteShader(*shader_ptr);
      delete shader_ptr;
    }
  };

  using ShaderGuard = std::unique_ptr<GLuint, ShaderDeleter>;

 public:
  Shader(const std::filesystem::path& path, GLenum type)
      : code_(loadFile(path)),
        type_(type),
        handle_(new GLuint(GLHPP_DETAIL_ERROR_HANDLER(glCreateShader, type))) {
    install();
  }

  Shader(Shader&& other) noexcept = default;
  Shader& operator=(Shader&& rhs) noexcept = default;

  auto id() const noexcept { return *handle_; }

 private:
  void install() {
    auto rbuf = code_.c_str();
    auto id = *handle_;
    GLHPP_DETAIL_ERROR_HANDLER(glShaderSource, id, 1, &rbuf, nullptr);
    GLHPP_DETAIL_ERROR_HANDLER(glCompileShader, id);

    GLint res;
    GLHPP_DETAIL_ERROR_HANDLER(glGetShaderiv, id, GL_COMPILE_STATUS, &res);
  }

 private:
  static std::string loadFile(const std::filesystem::path& path) {
    std::ifstream shader_file;
    shader_file.open(path, std::ios::in | std::ios::binary);
    if (!shader_file.is_open()) {
      auto what = "Failed to open file " + path.string();
      throw std::runtime_error(what);
    }
    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf();
    shader_file.close();
    return shader_stream.str();
  }

 private:
  std::string code_;
  GLenum type_;
  ShaderGuard handle_;
};

}
