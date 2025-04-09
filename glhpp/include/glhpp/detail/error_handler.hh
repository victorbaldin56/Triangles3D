#pragma once

#include <functional>

#include "GL/glew.h"

#include "glhpp/error.hh"

#define GLHPP_DETAIL_ERROR_HANDLER(func, ...) \
  glhpp::detail::errorHandler(func, #func, __VA_ARGS__)

namespace glhpp::detail {

template <typename Func, typename... Args>
auto errorHandler(Func func, const char* func_name, Args&&... args) {
  auto res = std::invoke(func, std::forward<Args>(args)...);
  auto e = glGetError();
  if (e != GL_NO_ERROR) {
    throw Error(func_name);
  }
  return res;
}
}
