#pragma once

#include <cmath>
#include <iostream>
#include <type_traits>

#include "comparator.hh"

namespace geometry {

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
struct Vector3D final {
  T x_ = NAN, y_ = NAN, z_ = NAN;

  bool valid() const noexcept {
    return std::isfinite(x_) && std::isfinite(y_) && std::isfinite(z_);
  }
  T norm2() const noexcept { return x_ * x_ + y_ * y_ + z_ * z_; }
  T norm() const noexcept { return std::sqrt(norm2()); }

  bool isClose(const Vector3D<T>& other) const noexcept {
    return comparator::isClose(x_, other.x_) &&
           comparator::isClose(y_, other.y_) &&
           comparator::isClose(z_, other.z_);
  }

  Vector3D<T>& operator+=(const Vector3D<T>& rhs) noexcept {
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    return *this;
  }

  Vector3D<T>& operator-=(const Vector3D<T>& rhs) noexcept {
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
    return *this;
  }

  Vector3D<T>& operator*=(T scalar) noexcept {
    x_ *= scalar;
    y_ *= scalar;
    z_ *= scalar;
    return *this;
  }

  Vector3D<T>& operator/=(T scalar) noexcept {
    x_ /= scalar;
    y_ /= scalar;
    z_ /= scalar;
    return *this;
  }

  Vector3D<T> operator+() const noexcept { return *this; }
  Vector3D<T> operator-() const noexcept {
    return Vector3D<T>(*this) *= static_cast<T>(-1);
  }

  Vector3D<T>& normalize() noexcept {
    T n = norm();
    x_ /= n;
    y_ /= n;
    z_ /= n;
    return *this;
  }

  static Vector3D<T> nullVector() noexcept { return {0, 0, 0}; }
};

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
Vector3D<T> operator+(Vector3D<T> a, const Vector3D<T>& b) noexcept {
  return a += b;
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
Vector3D<T> operator-(Vector3D<T> a, const Vector3D<T>& b) noexcept {
  return a -= b;
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
Vector3D<T> operator*(T scalar, Vector3D<T> v) noexcept {
  return v *= scalar;
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
Vector3D<T> operator*(const Vector3D<T>& v, T scalar) noexcept {
  return scalar * v;
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
Vector3D<T> operator/(Vector3D<T> v, T scalar) noexcept {
  return v /= scalar;
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
T dot(const Vector3D<T>& a, const Vector3D<T>& b) noexcept {
  return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
Vector3D<T> crossProduct(const Vector3D<T>& a, const Vector3D<T>& b) noexcept {
  return {a.y_ * b.z_ - a.z_ * b.y_,
          a.z_ * b.x_ - a.x_ * b.z_,
          a.x_ * b.y_ - a.y_ * b.x_};
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
T tripleProduct(const Vector3D<T>& a,
                const Vector3D<T>& b,
                const Vector3D<T>& c) noexcept {
  return dot(a, crossProduct(b, c));
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
bool collinear(const Vector3D<T>& a, const Vector3D<T>& b) noexcept {
  return crossProduct(a, b).isClose(Vector3D<T>::nullVector());
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
std::istream& operator>>(std::istream& is, Vector3D<T>& v) {
  is >> v.x_ >> v.y_ >> v.z_;
  return is;
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
std::ostream& operator<<(std::ostream& os, const Vector3D<T>& v) {
  os << v.x_ << v.y_ << v.z_;
  return os;
}

}  // namespace geometry
