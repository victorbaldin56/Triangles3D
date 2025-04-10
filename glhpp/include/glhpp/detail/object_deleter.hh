#pragma once

#include <algorithm>

#include "GL/glew.h"

namespace glhpp::detail {

template <void (*func)(GLuint)>
struct ObjectDeleter {
  struct pointer {
    GLuint x_;
    pointer(std::nullptr_t = nullptr) noexcept : x_(0) {}
    pointer(GLuint x) noexcept : x_(x) {}
    operator GLuint() const noexcept { return x_; }
    operator bool() const noexcept { return x_; }
    auto operator==(pointer other) const noexcept { return x_ == other.x_; }
    auto operator!=(pointer other) const noexcept { return !(*this == other); }
  };

  auto operator()(GLuint p) const noexcept { func(p); }
};
}
