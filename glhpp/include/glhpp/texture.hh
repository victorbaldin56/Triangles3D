#pragma once

#include <memory>

#include "light.hh"
#include "program.hh"
#include "shader.hh"

namespace glhpp {

class Texture final {
  static void deleteHandle(GLuint p) noexcept { glDeleteTextures(1, &p); }
  using Handle = std::unique_ptr<void, detail::ObjectDeleter<deleteHandle>>;

 public:
  Texture(const Light& light, unsigned vcount,
          const std::vector<Shader> shaders)
      : program_(shaders) {}

  Texture(Texture&& rhs) noexcept = default;
  Texture& operator=(Texture&& rhs) noexcept = default;

 private:
  Handle handle_;
  Program program_;
  unsigned width_ = 0;
  unsigned height_ = 0;
  glm::mat4 depth_mvp_;
  glm::vec3 light_dir_;
};

}
