#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace triangles_gl {

class Camera final {
 public:
  Camera(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up,
         float fov, float z_near_clip, float z_far_clip) noexcept
      : position_(pos),
        direction_(glm::normalize(dir)),
        up_(glm::normalize(up)),
        fov_(fov),
        z_near_clip_(z_near_clip),
        z_far_clip_(z_far_clip) {}

  auto translate(const glm::vec3& translation) noexcept {
    position_ += translation;
  }

  auto rotate(const glm::quat& q) noexcept {
    direction_ = direction_ * q;
    up_ = up_ * q;
  }

  const auto& getDirection() const noexcept { return direction_; }
  const auto& getUp() const noexcept { return up_; }
  auto getSideways() const noexcept { return glm::cross(direction_, up_); }
  auto getLookAt() const noexcept {
    return glm::lookAt(position_, position_ + direction_, up_);
  }
  auto getPerspective(unsigned width, unsigned height) const noexcept {
    return glm::perspective(fov_, static_cast<float>(width) / height,
                            z_near_clip_, z_far_clip_);
  }

 private:
  glm::vec3 position_;
  glm::vec3 direction_;
  glm::vec3 up_;
  float fov_, z_near_clip_, z_far_clip_;
};
}
