#pragma once

#include "glm/glm.hpp"

namespace triangles_gl {

class Camera final {
 public:
  Camera(glm::vec3 pos = {0, 0, 25}, glm::vec3 dir = {0, 0, -1},
         glm::vec3 up = {0, 1, 0}, float fov = glm::radians(45.f),
         float z_near_clip = 0.1f, float z_far_clip = 1000.f) noexcept
      : position_(pos),
        direction_(glm::normalize(dir)),
        up_(glm::normalize(up)),
        fov_(fov),
        z_near_clip_(z_near_clip),
        z_far_clip_(z_far_clip) {}

 private:
  glm::vec3 position_;
  glm::vec3 direction_;
  glm::vec3 up_;
  float fov_, z_near_clip_, z_far_clip_;
};
}
