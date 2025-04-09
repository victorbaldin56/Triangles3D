#pragma once

#include <optional>

#include "GL/glew.h"

namespace glhpp::detail {

template <typename Deleter>
class GlHandle {
 public:
  GlHandle(GLuint id) : id_(id) {}
  GlHandle(GlHandle&& other) noexcept
      : id_(std::exchange(other.id_, std::nullopt)) {}
  GlHandle& operator=(GlHandle&& rhs) noexcept { std::swap(id_, rhs.id_); }

  GlHandle(const GlHandle& other) = delete;
  GlHandle& operator=(const GlHandle& rhs) = delete;

  ~GlHandle() {
    if (id_.has_value()) {
      del_(id_);
    }
  }

  auto get() const noexcept { return id_.value(); }

 private:
  std::optional<GLuint> id_;
  Deleter del_;
};

}
