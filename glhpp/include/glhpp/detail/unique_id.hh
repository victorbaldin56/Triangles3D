#pragma once

#include <optional>

#include "GL/glew.h"

namespace glhpp::detail {

template <typename Deleter>
class UniqueId {
 public:
  UniqueId(GLuint id) : id_(id) {}
  UniqueId(UniqueId&& other) noexcept
      : id_(std::exchange(other.id_, std::nullopt)) {}
  UniqueId& operator=(UniqueId&& rhs) noexcept { std::swap(id_, rhs.id_); }

  UniqueId(const UniqueId& other) = delete;
  UniqueId& operator=(const UniqueId& rhs) = delete;

  ~UniqueId() {
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
