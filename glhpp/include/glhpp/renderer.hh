#pragma once

#include "program.hh"
#include "vertex_array.hh"

namespace glhpp {

class Renderer final {
 public:
  Renderer(const std::vector<Shader>& shaders) : program_(shaders) {}

 private:
  Program program_;
  VertexArray vertex_array_;
};
}
