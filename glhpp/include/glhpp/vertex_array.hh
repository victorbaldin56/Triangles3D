#pragma once

#include <cstddef>
#include <memory>

#include "GL/glew.h"
#include "detail/error_handler.hh"
#include "detail/object_deleter.hh"
#include "vertex.hh"

namespace glhpp {

class VertexArray final {
  static void deleteVao(GLuint p) noexcept { glDeleteVertexArrays(1, &p); }
  static void deleteVbo(GLuint p) noexcept { glDeleteBuffers(1, &p); }

  using Vao = std::unique_ptr<void, detail::ObjectDeleter<deleteVao>>;
  using Vbo = std::unique_ptr<void, detail::ObjectDeleter<deleteVbo>>;

 public:
  VertexArray(const std::vector<Vertex>& vertices)
      : vao_(genVao()), vbo_(genVbo()) {
    init(vertices);
  }
  VertexArray(VertexArray&& other) noexcept = default;
  VertexArray& operator=(VertexArray&& rhs) noexcept = default;

 private:
  void init(const std::vector<Vertex>& vertices) {
    bind();
    GLHPP_DETAIL_ERROR_HANDLER(glBufferData, GL_ARRAY_BUFFER,
                               vertices.size() * sizeof(Vertex),
                               vertices.data(), GL_STATIC_DRAW);
    setVao();
  }

  void bind() {
    GLHPP_DETAIL_ERROR_HANDLER(glBindVertexArray, vao_.get());
    GLHPP_DETAIL_ERROR_HANDLER(glBindBuffer, GL_ARRAY_BUFFER, vbo_.get());
  }

  void setVao() {
    GLHPP_DETAIL_ERROR_HANDLER(glEnableVertexAttribArray, 0);
    GLHPP_DETAIL_ERROR_HANDLER(glEnableVertexAttribArray, 1);
    GLHPP_DETAIL_ERROR_HANDLER(glEnableVertexAttribArray, 2);

    auto point_offset = reinterpret_cast<void*>(offsetof(Vertex, point));
    auto normal_offset = reinterpret_cast<void*>(offsetof(Vertex, normal));
    auto color_offset = reinterpret_cast<void*>(offsetof(Vertex, color_index));

    GLHPP_DETAIL_ERROR_HANDLER(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE,
                               sizeof(Vertex), point_offset);
    GLHPP_DETAIL_ERROR_HANDLER(glVertexAttribPointer, 1, 3, GL_FLOAT, GL_FALSE,
                               sizeof(Vertex), normal_offset);
    GLHPP_DETAIL_ERROR_HANDLER(glVertexAttribIPointer, 2, 1, GL_INT,
                               sizeof(Vertex), color_offset);
  }

  static GLuint genVao() {
    GLuint ret;
    GLHPP_DETAIL_ERROR_HANDLER(glGenVertexArrays, 1, &ret);
    return ret;
  }

  static GLuint genVbo() {
    GLuint ret;
    GLHPP_DETAIL_ERROR_HANDLER(glGenBuffers, 1, &ret);
    return ret;
  }

 private:
  Vao vao_;
  Vbo vbo_;
};
}
