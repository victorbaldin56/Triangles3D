#pragma once

#include "light.hh"
#include "program.hh"
#include "shadow_map.hh"
#include "vertex_array.hh"

namespace glhpp {

class Renderer final {
 public:
  Renderer(const std::vector<Shader>& main_shaders,
           const std::vector<Shader>& shadow_shaders,
           const std::vector<Vertex>& vertices, const Light& light,
           unsigned wnd_width, unsigned wnd_height, GLenum figure_type)
      : program_(main_shaders),
        vertex_array_(vertices),
        shadow_map_(light, vertices.size(), shadow_shaders, figure_type),
        figure_type_(figure_type) {
    init(vertices, light, wnd_width, wnd_height);
  }

  auto render(const glm::mat4& perspective, const glm::mat4& look_at) const {
    setUniformMvp(perspective, look_at);
    shadow_map_.setUniformDepthBiasMvp(program_.id());
    GLHPP_DETAIL_ERROR_HANDLER(glClear,
                               GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderCw();
    renderCcw();
  }

  auto resize(unsigned width, unsigned height) const {
    GLHPP_DETAIL_ERROR_HANDLER(glViewport, 0, 0, width, height);
  }

 private:
  void init(const std::vector<Vertex>& vertices, const Light& light,
            unsigned wnd_width, unsigned wnd_height) {
    GLHPP_DETAIL_ERROR_HANDLER(glUseProgram, program_.id());
    resize(wnd_width, wnd_height);

    shadow_map_.setUniformShadowMap(program_.id());
    shadow_map_.setUniformLightDirection(program_.id());
    setUniformColors();
  }

  void setUniformMvp(const glm::mat4& perspective,
                     const glm::mat4& look_at) const {
    glm::mat4 model(1.f);
    auto mvp = perspective * look_at * model;
    GLHPP_DETAIL_ERROR_HANDLER(glUseProgram, program_.id());
    GLHPP_DETAIL_ERROR_HANDLER(
        glUniformMatrix4fv,
        GLHPP_DETAIL_ERROR_HANDLER(glGetUniformLocation, program_.id(), "mvp"),
        1, GL_FALSE, &mvp[0][0]);
  }

  void setUniformColors() const {
    glm::vec3 colors[2]{{0.f, 0.f, 1.f},   // blue
                        {1.f, 0.f, 0.f}};  // red
    GLHPP_DETAIL_ERROR_HANDLER(
        glUniform3fv,
        GLHPP_DETAIL_ERROR_HANDLER(glGetUniformLocation, program_.id(),
                                   "colors"),
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
    GLHPP_DETAIL_ERROR_HANDLER(glDrawArrays, figure_type_, 0,
                               shadow_map_.vertexCount());
  }

  void renderCcw() const {
    GLHPP_DETAIL_ERROR_HANDLER(glFrontFace, GL_CCW);
    setUniformIsCw(false);
    GLHPP_DETAIL_ERROR_HANDLER(glDrawArrays, figure_type_, 0,
                               shadow_map_.vertexCount());
  }

 private:
  Program program_;
  VertexArray vertex_array_;
  ShadowMap shadow_map_;

  GLenum figure_type_;
};
}
