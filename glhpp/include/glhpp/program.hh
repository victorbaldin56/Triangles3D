#pragma once

#include "shader.hh"

namespace glhpp {

class Program final {
  GLuint program_id_;
  std::vector<Shader> shaders_;

 public:
};

}
