#pragma once

#include <cstddef>
#include <memory>

#include "GL/glew.h"
#include "detail/error_handler.hh"
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
  VertexArray(const std::vector<Vertex> vertices) { init(vertices); }
  VertexArray(VertexArray&& other) noexcept = default;
  VertexArray& operator=(VertexArray&& rhs) noexcept = default;

 private:
  void init(const std::vector<Vertex> vertices) {
    bind();
    GLHPP_DETAIL_ERROR_HANDLER(glBufferData, GL_ARRAY_BUFFER,
                               vertices.size() * sizeof(Vertex),
                               vertices.data(), GL_STATIC_DRAW);
    setVao();
  }

  void bind() {
    GLHPP_DETAIL_ERROR_HANDLER(glGenVertexArrays, 1, vao_.get());
    GLHPP_DETAIL_ERROR_HANDLER(glGenBuffers, 1, vbo_.get());

    GLHPP_DETAIL_ERROR_HANDLER(glBindVertexArray, *vao_);
    GLHPP_DETAIL_ERROR_HANDLER(glBindBuffer, GL_ARRAY_BUFFER, *vbo_);
  }

  void setVao() {
    GLHPP_DETAIL_ERROR_HANDLER(glEnableVertexAttribArray, 0);
    GLHPP_DETAIL_ERROR_HANDLER(glEnableVertexAttribArray, 1);
    GLHPP_DETAIL_ERROR_HANDLER(glEnableVertexAttribArray, 2);

    // OpenGL forces to do this
    auto point_offset = reinterpret_cast<void*>(offsetof(Vertex, point));
    auto normal_offset = reinterpret_cast<void*>(offsetof(Vertex, normal));
    auto color_offset = reinterpret_cast<void*>(offsetof(Vertex, color));

    GLHPP_DETAIL_ERROR_HANDLER(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE,
                               sizeof(Vertex), point_offset);
    GLHPP_DETAIL_ERROR_HANDLER(glVertexAttribPointer, 1, 3, GL_FLOAT, GL_FALSE,
                               sizeof(Vertex), point_offset);
    GLHPP_DETAIL_ERROR_HANDLER(glVertexAttribIPointer, 2, 1, GL_BYTE,
                               sizeof(Vertex), point_offset);
  }

 private:
  Vao vao_;
  Vbo vbo_;
};
}
