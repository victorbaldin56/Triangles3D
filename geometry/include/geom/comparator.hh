#pragma once

#include <numeric>

namespace comparator {

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
inline T absoluteTolerance() noexcept {
  return std::numeric_limits<T>::is_exact ? static_cast<T>(0)
                                          : static_cast<T>(1e-5);
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
inline T relativeTolerance() noexcept {
  return std::numeric_limits<T>::is_exact ? static_cast<T>(0)
                                          : static_cast<T>(1e-4);
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
inline bool isClose(const T& a, const T& b,
                    const T& abs_tol = absoluteTolerance<T>(),
                    const T& rel_tol = relativeTolerance<T>()) noexcept {
  return std::abs(a - b) <=
         std::max(rel_tol * std::max(std::abs(a), std::abs(b)), abs_tol);
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
inline bool isLessClose(const T& a, const T& b,
                        const T& abs_tol = absoluteTolerance<T>(),
                        const T& rel_tol = relativeTolerance<T>()) noexcept {
  return isClose(a, b, abs_tol, rel_tol) || a < b;
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
inline bool isGreaterClose(const T& a, const T& b,
                           const T& abs_tol = absoluteTolerance<T>(),
                           const T& rel_tol = relativeTolerance<T>()) noexcept {
  return isClose(a, b, abs_tol, rel_tol) || a > b;
}

}  // namespace comparator
