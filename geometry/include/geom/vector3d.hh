#ifndef TRIANGLES_3D_GEOMETRY_VECTOR_3D_HH_
#define TRIANGLES_3D_GEOMETRY_VECTOR_3D_HH_

#include <cmath>
#include <type_traits>
#include <iostream>

#include "comparator.hh"

namespace geometry {

template <typename T>
struct Vector3D {
  static_assert(std::is_floating_point<T>(),
                "Only floating-point type can be used.");

  T x_ = NAN, y_ = NAN, z_ = NAN;

  bool valid() const {
    return std::isfinite(x_) && std::isfinite(y_) && std::isfinite(z_);
  }

  T norm2() const {
    return x_*x_ + y_*y_ + z_*z_;
  }

  T norm() const {
    return std::sqrt(norm2());
  }

  bool equal(const Vector3D<T>& other) const {
    return comparator::isClose(x_, other.x_) &&
           comparator::isClose(y_, other.y_) &&
           comparator::isClose(z_, other.z_);
  }

  Vector3D<T>& operator+=(const Vector3D<T>& rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    return *this;
  }

  Vector3D<T>& operator-=(const Vector3D<T>& rhs) {
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
    return *this;
  }

  Vector3D<T>& operator*=(T scalar) {
    x_*=scalar;
    y_*=scalar;
    z_*=scalar;
    return *this;
  }

  Vector3D<T>& operator/=(T scalar) {
    x_/=scalar;
    y_/=scalar;
    z_/=scalar;
    return *this;
  }

  Vector3D<T> operator+() const {
    return *this;
  }

  Vector3D<T> operator-() const {
    return Vector3D<T>{*this}*=static_cast<T>(-1);
  }

  Vector3D<T>& normalize() {
    T n = norm();
    x_/=n;
    y_/=n;
    z_/=n;
    return *this;
  }

  static Vector3D<T> zeroVector() {
    return {0, 0, 0};
  }
};

template <typename T>
Vector3D<T> operator+(Vector3D<T> a, const Vector3D<T>& b) {
  return a += b;
}

template <typename T>
Vector3D<T> operator-(Vector3D<T> a, const Vector3D<T>& b) {
  return a -= b;
}

template <typename T>
Vector3D<T> operator*(T scalar, Vector3D<T> v) {
  return v*=scalar;
}

template <typename T>
Vector3D<T> operator*(const Vector3D<T>& v, T scalar) {
  return scalar*v;
}

template <typename T>
Vector3D<T> operator/(Vector3D<T> v, T scalar) {
  return v/=scalar;
}

template <typename T>
T dot(const Vector3D<T>& a, const Vector3D<T>& b) {
  return a.x_*b.x_ + a.y_*b.y_ + a.z_*b.z_;
}

template <typename T>
Vector3D<T> crossProduct(const Vector3D<T>& a, const Vector3D<T>& b) {
  return {a.y_*b.z_ - a.z_*b.y_,
          a.z_*b.x_ - a.x_*b.z_,
          a.x_*b.y_ - a.y_*b.x_};
}

template <typename T>
T tripleProduct(const Vector3D<T>& a,
                const Vector3D<T>& b,
                const Vector3D<T>& c) {
  return dot(a, crossProduct(b, c));
}

template <typename T>
bool collinear(const Vector3D<T>& a, const Vector3D<T>& b) {
  return crossProduct(a, b).equal(Vector3D<T>::zeroVector());
}

template <typename T>
std::istream& operator>>(std::istream& is, Vector3D<T>& v) {
  is >> v.x_ >> v.y_ >> v.z_;
  return is;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3D<T>& v) {
  os << v.x_ << v.y_ << v.z_;
  return os;
}

}

#endif // TRIANGLES_3D_GEOMETRY_VECTOR_3D_HH_
