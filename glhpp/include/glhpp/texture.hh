#pragma once

#include <memory>

#include "shader.hh"

namespace glhpp {

class Texture final {
  struct Deleter {
    auto operator()(GLuint* p) const noexcept {
      glDeleteTextures(1, p);
      delete p;
    }
  };
};

}
