#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace glhpp {

struct Light {
  unsigned width;
  unsigned height;
  glm::vec3 dir;
  glm::vec3 pos;
  glm::vec3 up;
  glm::mat4 projection_matrix;
};

}
