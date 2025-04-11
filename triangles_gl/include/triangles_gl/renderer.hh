#pragma once

#include "glhpp/light.hh"
#include "glhpp/program.hh"
#include "glhpp/shadow_map.hh"
#include "glhpp/vertex_array.hh"

namespace triangles_gl {

class Renderer final {
 public:
  Renderer(const std::vector<glhpp::Shader>& main_shaders,
           const std::vector<glhpp::Shader>& shadow_shaders,
           const std::vector<glhpp::Vertex>& vertices,
           const glhpp::Light& light, unsigned wnd_width, unsigned wnd_height)
      : program_(main_shaders),
        vertex_array_(vertices),
        texture_(light, vertices.size(), shadow_shaders, GL_TRIANGLES) {}

  auto render(const glm::mat4& perspective, const glm::mat4& look_at) const {
    setUniformMvp(perspective, look_at);
    texture_.setUniformDepthBiasMvp(program_.id());
    GLHPP_DETAIL_ERROR_HANDLER(glClear,
                               GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderCw();
    renderCcw();
  }

  auto resize(unsigned width, unsigned height) const {
    GLHPP_DETAIL_ERROR_HANDLER(glViewport, 0, 0, width, height);
    GLHPP_DETAIL_ERROR_HANDLER(glMatrixMode, GL_PROJECTION);
    GLHPP_DETAIL_ERROR_HANDLER(glLoadIdentity);
    GLHPP_DETAIL_ERROR_HANDLER(glOrtho, 0, width, 0, height, -1.0, 1.0);
    GLHPP_DETAIL_ERROR_HANDLER(glMatrixMode, GL_MODELVIEW);
  }

 private:
  void init(const std::vector<glhpp::Vertex>& vertices, glhpp::Light& light,
            unsigned wnd_width, unsigned wnd_height) {
    GLHPP_DETAIL_ERROR_HANDLER(glUseProgram, program_.id());
    resize(wnd_width, wnd_height);

    texture_.setUniformShadowMap(program_.id());
    texture_.setUniformLightDirection(program_.id());
    setUniformColors();
  }

  void setUniformMvp(const glm::mat4& perspective,
                     const glm::mat4& look_at) const {
    glm::mat4 model(1.f);
    auto mvp = perspective * look_at * model;
    GLHPP_DETAIL_ERROR_HANDLER(
        glUniformMatrix4fv,
        GLHPP_DETAIL_ERROR_HANDLER(glGetUniformLocation, program_.id(), "mvp"),
        1, GL_FALSE, &mvp[0][0]);
  }

  void setUniformColors() const {
    glm::vec3 colors[2] = {{0.f, 0.f, 1.f}, {1.f, 0.f, 0.f}};
    GLHPP_DETAIL_ERROR_HANDLER(
        glUniform3fv,
        GLHPP_DETAIL_ERROR_HANDLER(glGetUniformLocation, program_.id(),
                                   "color"),
        2, &colors[0][0]);
  }

  /**
   * Clock-wise or counter-clock wise (face culling)
   */
  void setUniformIsCw(bool is_cw) const {
    GLHPP_DETAIL_ERROR_HANDLER(
        glUniform1i,
        GLHPP_DETAIL_ERROR_HANDLER(glGetUniformLocation, program_.id(), "cw"),
        is_cw);
  }

  void renderCw() const {
    GLHPP_DETAIL_ERROR_HANDLER(glFrontFace, GL_CW);
    setUniformIsCw(true);
    GLHPP_DETAIL_ERROR_HANDLER(glDrawArrays, GL_TRIANGLES, 0,
                               texture_.vertexCount());
  }

  void renderCcw() const {
    GLHPP_DETAIL_ERROR_HANDLER(glFrontFace, GL_CCW);
    setUniformIsCw(false);
    GLHPP_DETAIL_ERROR_HANDLER(glDrawArrays, GL_TRIANGLES, 0,
                               texture_.vertexCount());
  }

 private:
  glhpp::Program program_;
  glhpp::VertexArray vertex_array_;
  glhpp::ShadowMap texture_;
};
}  // namespace triangles_gl
