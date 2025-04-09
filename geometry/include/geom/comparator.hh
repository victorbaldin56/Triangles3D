#pragma once

#include <numeric>

namespace comparator {

template <typename T>
inline constexpr auto kAbsTol = static_cast<T>(0);
template <>
inline constexpr auto kAbsTol<float> = 1e-4f;
template <>
inline constexpr auto kAbsTol<double> = 1e-5;

template <typename T>
inline constexpr auto kRelTol = static_cast<T>(0);
template <>
inline constexpr auto kRelTol<float> = 1e-4f;
template <>
inline constexpr auto kRelTol<double> = 1e-8;

template <typename T>
inline bool isClose(const T& a, const T& b, const T& abs_tol = kAbsTol<T>,
                    const T& rel_tol = kRelTol<T>) noexcept {
  return std::abs(a - b) <=
         std::max(rel_tol * std::max(std::abs(a), std::abs(b)), abs_tol);
}

template <typename T>
inline bool isLessClose(const T& a, const T& b, const T& abs_tol = kAbsTol<T>,
                        const T& rel_tol = kRelTol<T>) noexcept {
  return isClose(a, b, abs_tol, rel_tol) || a < b;
}

template <typename T>
inline bool isGreaterClose(const T& a, const T& b,
                           const T& abs_tol = kAbsTol<T>,
                           const T& rel_tol = kRelTol<T>) noexcept {
  return isClose(a, b, abs_tol, rel_tol) || a > b;
}

}  // namespace comparator
