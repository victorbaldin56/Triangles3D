#pragma once

#include <cstddef>
#include <memory>

#include "GL/glew.h"
#include "detail/error_handler.hh"
#include "detail/object_deleter.hh"

namespace glhpp {

class VertexArray final {
  static void deleteVao(GLuint p) noexcept { glDeleteVertexArrays(1, &p); }
  static void deleteVbo(GLuint p) noexcept { glDeleteBuffers(1, &p); }

  using Vao = std::unique_ptr<void, detail::ObjectDeleter<deleteVao>>;
  using Vbo = std::unique_ptr<void, detail::ObjectDeleter<deleteVbo>>;

 public:
  VertexArray(const void* buffer, std::size_t size)
      : vao_(genVao()), vbo_(genVbo()) {
    init(buffer, size);
  }
  VertexArray(VertexArray&& other) noexcept = default;
  VertexArray& operator=(VertexArray&& rhs) noexcept = default;

  void setAttribute(GLuint index, GLsizei size, GLenum type,
                    GLboolean normalized, GLsizei stride,
                    std::size_t offset) const {
    GLHPP_DETAIL_ERROR_HANDLER(glEnableVertexAttribArray, index);
    GLHPP_DETAIL_ERROR_HANDLER(glVertexAttribPointer, index, size, type,
                               normalized, stride,
                               reinterpret_cast<GLvoid*>(offset));
  }

 private:
  void init(const void* buffer, std::size_t size) {
    bind();
    GLHPP_DETAIL_ERROR_HANDLER(glBufferData, GL_ARRAY_BUFFER, size, buffer,
                               GL_STATIC_DRAW);
  }

  void bind() {
    GLHPP_DETAIL_ERROR_HANDLER(glBindVertexArray, vao_.get());
    GLHPP_DETAIL_ERROR_HANDLER(glBindBuffer, GL_ARRAY_BUFFER, vbo_.get());
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
