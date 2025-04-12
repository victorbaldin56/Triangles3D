#pragma once

#include "shader.hh"

namespace glhpp {

class Program final {
  static void deleteHandle(GLuint p) noexcept {
    constexpr GLsizei kMaxCount = 2;
    GLsizei count;
    std::array<GLuint, kMaxCount> attached_shaders;
    glGetAttachedShaders(p, kMaxCount, &count, attached_shaders.data());
    for (int i = 0; i < count; ++i) {
      glDetachShader(p, attached_shaders[i]);
    }
    glDeleteProgram(p);
  }
  using Handle = std::unique_ptr<void, detail::ObjectDeleter<deleteHandle>>;

 public:
  Program(const std::vector<Shader>& shaders)
      : handle_(GLHPP_DETAIL_ERROR_HANDLER(glCreateProgram)) {
    auto id = handle_.get();
    std::for_each(shaders.begin(), shaders.end(), [&](auto&& s) {
      GLHPP_DETAIL_ERROR_HANDLER(glAttachShader, id, s.id());
    });
    GLHPP_DETAIL_ERROR_HANDLER(glLinkProgram, id);

    GLint res;
    GLHPP_DETAIL_ERROR_HANDLER(glGetProgramiv, id, GL_LINK_STATUS, &res);
    handleProgramiv(id, res);
  }

  Program(Program&& other) noexcept = default;
  Program& operator=(Program&& rhs) noexcept = default;

  auto id() const noexcept { return handle_.get(); }

 private:
  static void handleProgramiv(GLuint program_id, GLint res) {
    if (res) {
      return;
    }

    std::stringstream build_log;
    build_log << "Error building program\n";
    int log_length;
    GLHPP_DETAIL_ERROR_HANDLER(glGetProgramiv, program_id, GL_INFO_LOG_LENGTH,
                               &log_length);
    if (log_length > 0) {
      auto err_msg = std::make_unique<char[]>(log_length + 1);
      GLHPP_DETAIL_ERROR_HANDLER(glGetProgramInfoLog, program_id, log_length,
                                 nullptr, err_msg.get());
      build_log << err_msg.get();
    }

    throw Error(build_log.str());
  }

 private:
  Handle handle_;
};

}
