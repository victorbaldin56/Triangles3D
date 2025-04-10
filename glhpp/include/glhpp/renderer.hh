#pragma once

#include "light.hh"
#include "program.hh"
#include "vertex_array.hh"

namespace glhpp {

class Renderer final {
 public:
  Renderer(const std::vector<Shader>& shaders,
           const std::vector<Shader>& shadow_shaders,
           const std::vector<Vertex>& vertices, const Light& light)
      : program_(shaders), vertex_array_(vertices) {}

 private:
  Program program_;
  VertexArray vertex_array_;
};
}
