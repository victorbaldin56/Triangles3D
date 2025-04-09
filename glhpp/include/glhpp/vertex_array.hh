#pragma once

#include <cstddef>
#include <memory>

#include "GL/glew.h"

namespace glhpp {

class VertexArray final {
  struct VaoDeleter {
    auto operator()(GLuint* ptr) const noexcept {
      glDeleteVertexArrays(1, ptr);
      delete ptr;
    }
  };

  struct VboDeleter {
    auto operator()(GLuint* ptr) const noexcept {
      glDeleteBuffers(1, ptr);
      delete ptr;
    }
  };

  using Vao = std::unique_ptr<GLuint, VaoDeleter>;
  using Vbo = std::unique_ptr<GLuint, VboDeleter>;

 public:
  VertexArray(const void* vertices, std::size_t sz) {
    // TODO
  }

 private:
  Vao vao_;
  Vbo vbo_;
};
}
