#pragma once

#include <memory>

#include "light.hh"
#include "shader.hh"

namespace glhpp {

class Texture final {
  static void deleteHandle(GLuint p) noexcept { glDeleteTextures(1, &p); }
  using Handle = std::unique_ptr<void, detail::ObjectDeleter<deleteHandle>>;

 public:
  Texture(const Light& light, unsigned vcount,
          const std::vector<Shader> shaders);

 private:
  Handle handle_;
};

}
