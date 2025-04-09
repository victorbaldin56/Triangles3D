#pragma once

#include <cstddef>
#include <memory>

#include "GL/glew.h"
#include "geom/vector3d.hh"

namespace glhpp {

struct Vertex final {
  using Point = geometry::Vector3D<GLfloat>;

  Point point;
  Point normal;
  GLbyte color;
};

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
  VertexArray(const std::vector<Vertex> vertices) {
    // TODO
  }

 private:
  Vao vao_;
  Vbo vbo_;
};
}
