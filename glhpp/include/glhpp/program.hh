#pragma once

#include "shader.hh"

namespace glhpp {

class Program final {
  struct ProgramDeleter {
    auto operator()(GLuint id) const noexcept { glDeleteProgram(id); }
  };

  using ProgramGuard = detail::GlHandle<ProgramDeleter>;

 public:
  Program(const std::vector<Shader>& shaders)
      : handle_(GLHPP_DETAIL_ERROR_HANDLER(glCreateProgram)),
        shaders_(shaders) {
    auto id = handle_.get();
    std::for_each(shaders_.begin(), shaders_.end(), [&](auto&& s) {
      GLHPP_DETAIL_ERROR_HANDLER(glAttachShader, id, s.id());
    });
    GLHPP_DETAIL_ERROR_HANDLER(glLinkProgram, id);

    GLint res;
    GLHPP_DETAIL_ERROR_HANDLER(glGetProgramiv, id, GL_LINK_STATUS, &res);
  }

  Program(Program&& other) noexcept = default;
  Program& operator=(Program&& rhs) noexcept = default;

  auto id() const noexcept { return handle_.get(); }

 private:
  ProgramGuard handle_;
  std::vector<Shader> shaders_;
};

}
