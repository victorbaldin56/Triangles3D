#pragma once

#include <cassert>
#include <cstddef>
#include <memory>

#include "light.hh"
#include "program.hh"
#include "shader.hh"

namespace glhpp {

class ShadowMap final {
  static void deleteHandle(GLuint p) noexcept { glDeleteTextures(1, &p); }
  using Handle = std::unique_ptr<void, detail::ObjectDeleter<deleteHandle>>;

  static void deleteFrameBuffer(GLuint p) noexcept {
    glDeleteFramebuffers(1, &p);
  }
  using Framebuffer =
      std::unique_ptr<void, detail::ObjectDeleter<deleteFrameBuffer>>;

 public:
  ShadowMap(const Light& light, std::size_t vcount,
            const std::vector<Shader>& shaders, int figure_type)
      : handle_(genTexture()),
        program_(shaders),
        light_dir_(light.dir),
        width_(light.width),
        height_(light.height),
        vertex_count_(vcount) {
    drawShadows(light, figure_type);
  }

  ShadowMap(ShadowMap&& rhs) noexcept = default;
  ShadowMap& operator=(ShadowMap&& rhs) noexcept = default;

  auto setUniformShadowMap(GLuint program_id) const {
    GLHPP_DETAIL_ERROR_HANDLER(glActiveTexture, GL_TEXTURE0);
    GLHPP_DETAIL_ERROR_HANDLER(glBindTexture, GL_TEXTURE_2D, handle_.get());
    GLHPP_DETAIL_ERROR_HANDLER(
        glUniform1i,
        GLHPP_DETAIL_ERROR_HANDLER(glGetUniformLocation, program_id,
                                   "shadow_map"),
        0);
  }

  auto setUniformLightDirection(GLuint program_id) const {
    auto light_dir_normalized = glm::normalize(light_dir_);
    GLHPP_DETAIL_ERROR_HANDLER(
        glUniform3f,
        GLHPP_DETAIL_ERROR_HANDLER(glGetUniformLocation, program_id,
                                   "light_dir"),
        light_dir_normalized.x, light_dir_normalized.y, light_dir_normalized.z);
  }

  auto setUniformDepthBiasMvp(GLuint program_id) const {
    // clang-format off
    glm::mat4 bias_matrix(0.5, 0.0, 0.0, 0.0,
                          0.0, 0.5, 0.0, 0.0,
                          0.0, 0.0, 0.5, 0.0,
                          0.5, 0.5, 0.5, 1.0);
    // clang-format on
    glm::mat4 depth_bias_mvp = bias_matrix * depth_mvp_;
    GLHPP_DETAIL_ERROR_HANDLER(
        glUniformMatrix4fv,
        GLHPP_DETAIL_ERROR_HANDLER(glGetUniformLocation, program_id,
                                   "depth_bias_mvp"),
        1, GL_FALSE, &depth_bias_mvp[0][0]);
  }

  auto vertexCount() const noexcept { return vertex_count_; }

 private:
  void drawShadows(const Light& light, int figure_type) {
    bind();
    Framebuffer fb(genFramebuffer());
    GLHPP_DETAIL_ERROR_HANDLER(glBindFramebuffer, GL_FRAMEBUFFER, fb.get());
    GLHPP_DETAIL_ERROR_HANDLER(glDrawBuffer, GL_NONE);
    GLHPP_DETAIL_ERROR_HANDLER(glReadBuffer, GL_NONE);
    GLHPP_DETAIL_ERROR_HANDLER(glFramebufferTexture2D, GL_FRAMEBUFFER,
                               GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                               handle_.get(), 0);

    setUniformDepthMvp(light);

    GLHPP_DETAIL_ERROR_HANDLER(glViewport, 0, 0, width_, height_);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    GLHPP_DETAIL_ERROR_HANDLER(glClear,
                               GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLHPP_DETAIL_ERROR_HANDLER(glDrawArrays, figure_type, 0, vertex_count_);
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

  void setUniformDepthMvp(const Light& light) {
    auto depth_view_matrix = glm::lookAt(light.pos, light.dir, light.up);
    glm::mat4 depth_model_matrix(1.f);
    depth_mvp_ =
        light.projection_matrix * depth_view_matrix * depth_model_matrix;
    GLHPP_DETAIL_ERROR_HANDLER(glUseProgram, program_.id());
    GLHPP_DETAIL_ERROR_HANDLER(
        glUniformMatrix4fv,
        GLHPP_DETAIL_ERROR_HANDLER(glGetUniformLocation, program_.id(),
                                   "depth_mvp"),
        1, GL_FALSE, &depth_mvp_[0][0]);
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
  unsigned width_;
  unsigned height_;
  std::size_t vertex_count_;
  glm::mat4 depth_mvp_;
  glm::vec3 light_dir_;
};
}
