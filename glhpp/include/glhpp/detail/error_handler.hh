#pragma once

#include <functional>
#include <iomanip>
#include <sstream>
#include <type_traits>

#include "GL/glew.h"
#include "glhpp/error.hh"

#define GLHPP_DETAIL_ERROR_HANDLER(func, ...)                  \
  glhpp::detail::errorHandler(func, #func, __FILE__, __func__, \
                              __LINE__ __VA_OPT__(, ) __VA_ARGS__)

namespace glhpp::detail {

inline auto checkError(const char* func_name, const char* file,
                       const char* caller_func, int line) {
  auto e = glGetError();
  if (e != GL_NO_ERROR) {
    std::stringstream ss;
    ss << std::string("Error: ") << func_name << " called from " << file << ":"
       << std::to_string(line) << ", " << caller_func
       << " failed with OpenGL error 0x" << std::hex << e;
    throw Error(ss.str());
  }
}

template <typename Func, typename... Args>
auto errorHandler(Func func, const char* func_name, const char* file,
                  const char* caller_func, int line, Args&&... args) {
  if constexpr (std::is_void_v<std::invoke_result_t<Func, Args...>>) {
    std::invoke(func, std::forward<Args>(args)...);
    checkError(func_name, file, caller_func, line);
  } else {
    auto res = std::invoke(func, std::forward<Args>(args)...);
    checkError(func_name, file, caller_func, line);
    return res;
  }
}
}
