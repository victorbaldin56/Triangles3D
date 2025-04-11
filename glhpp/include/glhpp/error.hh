#pragma once

#include <stdexcept>

namespace glhpp {

class Error : public std::runtime_error {
 public:
  Error(const char* what) noexcept : std::runtime_error(what) {}
  Error(const std::string& what) noexcept : std::runtime_error(what) {}
};

}
