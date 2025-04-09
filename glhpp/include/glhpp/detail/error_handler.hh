#pragma once

#include <functional>
#include <type_traits>

#include "GL/glew.h"
#include "glhpp/error.hh"

#define GLHPP_DETAIL_ERROR_HANDLER(func, ...) \
  glhpp::detail::errorHandler(func, #func __VA_OPT__(, ) __VA_ARGS__)

namespace glhpp::detail {

inline auto checkError(const char* func_name) {
  auto e = glGetError();
  if (e != GL_NO_ERROR) {
    throw Error(func_name);
  }
}

template <typename Func, typename... Args>
auto errorHandler(Func func, const char* func_name, Args&&... args) {
  if constexpr (std::is_void_v<std::invoke_result_t<Func, Args...>>) {
    std::invoke(func, std::forward<Args>(args)...);
    checkError(func_name);
  } else {
    auto res = std::invoke(func, std::forward<Args>(args)...);
    checkError(func_name);
    return res;
  }
}
}
