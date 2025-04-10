#pragma once

#include <cstddef>
#include <memory>

#include "light.hh"
#include "program.hh"
#include "shader.hh"

namespace glhpp {

class Texture final {
  static void deleteHandle(GLuint p) noexcept { glDeleteTextures(1, &p); }
  using Handle = std::unique_ptr<void, detail::ObjectDeleter<deleteHandle>>;

  static void deleteFrameBuffer(GLuint p) noexcept {
    glDeleteFramebuffers(1, &p);
  }
  using Framebuffer =
      std::unique_ptr<void, detail::ObjectDeleter<deleteFrameBuffer>>;

 public:
  Texture(const Light& light, std::size_t vcount,
          const std::vector<Shader> shaders, int figure_type)
      : handle_(genTexture()),
        program_(shaders),
        light_dir_(light.dir),
        width_(light.width),
        height_(light.height) {
    drawShadows(light, vcount, figure_type);
  }

  Texture(Texture&& rhs) noexcept = default;
  Texture& operator=(Texture&& rhs) noexcept = default;

 private:
  void drawShadows(const Light& light, std::size_t vcount, int figure_type) {
    bind();
    Framebuffer fb(genFramebuffer());
    GLHPP_DETAIL_ERROR_HANDLER(glBindFramebuffer, GL_FRAMEBUFFER, fb.get());
    GLHPP_DETAIL_ERROR_HANDLER(glDrawBuffer, GL_NONE);
    GLHPP_DETAIL_ERROR_HANDLER(glReadBuffer, GL_NONE);
    GLHPP_DETAIL_ERROR_HANDLER(glFramebufferTexture2D, GL_FRAMEBUFFER,
                               GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                               handle_.get(), 0);

    setUniformMvp(program_.id(), light);

    GLHPP_DETAIL_ERROR_HANDLER(glViewport, 0, 0, width_, height_);
    GLHPP_DETAIL_ERROR_HANDLER(glClear,
                               GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLHPP_DETAIL_ERROR_HANDLER(glDrawArrays, figure_type, 0, vcount);
    GLHPP_DETAIL_ERROR_HANDLER(glBindFramebuffer, GL_FRAMEBUFFER, 0);
  }

  void bind() {
    GLHPP_DETAIL_ERROR_HANDLER(glBindTexture, GL_TEXTURE_2D, handle_.get());
    GLHPP_DETAIL_ERROR_HANDLER(glTexImage2D, GL_TEXTURE_2D, 0,
                               GL_DEPTH_COMPONENT, width_, height_, 0,
                               GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    GLHPP_DETAIL_ERROR_HANDLER(glTexParameteri, GL_TEXTURE_2D,
                               GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GLHPP_DETAIL_ERROR_HANDLER(glTexParameteri, GL_TEXTURE_2D,
                               GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GLHPP_DETAIL_ERROR_HANDLER(glTexParameteri, GL_TEXTURE_2D,
                               GL_TEXTURE_WRAP_S, GL_REPEAT);
    GLHPP_DETAIL_ERROR_HANDLER(glTexParameteri, GL_TEXTURE_2D,
                               GL_TEXTURE_WRAP_T, GL_REPEAT);
  }

  void setUniformMvp(GLuint program_id, const Light& light) {
    auto depth_view_matrix = glm::lookAt(light.pos, light.dir, light.up);
    glm::mat4 depth_model_matrix(1.f);
    mvp_ = light.projection_matrix * depth_view_matrix * depth_model_matrix;
    GLHPP_DETAIL_ERROR_HANDLER(glUniformMatrix4fv,
                               glGetUniformLocation(program_id, "mvp"), 1,
                               GL_FALSE, &mvp_[0][0]);
  }

  static GLuint genTexture() {
    GLuint ret;
    GLHPP_DETAIL_ERROR_HANDLER(glGenTextures, 1, &ret);
    return ret;
  }

  static GLuint genFramebuffer() {
    GLuint ret;
    GLHPP_DETAIL_ERROR_HANDLER(glGenFramebuffers, 1, &ret);
    return ret;
  }

 private:
  Handle handle_;
  Program program_;
  unsigned width_ = 0;
  unsigned height_ = 0;
  glm::mat4 mvp_;
  glm::vec3 light_dir_;
};

}
