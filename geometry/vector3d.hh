#ifndef TRIANGLES_3D_GEOMETRY_VECTOR_3D_HH_
#define TRIANGLES_3D_GEOMETRY_VECTOR_3D_HH_

#include <cmath>

#include "numeric.hh"

namespace geometry {

template <typename T>
struct Vector3D {
  T x_, y_, z_;

  T norm2() const {
    return x_*x_ + y_*y_ + z_*z_;
  }

  T norm() const {
    return std::sqrt(norm2());
  }

  bool isClose(const Vector3D<T>& other) const {
    return numeric::isClose(x_, other.x_) &&
           numeric::isClose(y_, other.y_) &&
           numeric::isClose(z_, other.z_);
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

  Vector3D<T> operator+() const {
    return *this;
  }

  Vector3D<T> operator-() const {
    return Vector3D<T>{*this}*=static_cast<T>(-1);
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
T dot(const Vector3D<T>& a, const Vector3D<T>& b) {
  return a.x_*b.x_ + a.y_*b.y_ + a.z_*b.z_;
}

template <typename T>
Vector3D<T> crossProduct(const Vector3D<T>& a, const Vector3D<T>& b) {
  return {a.y_*b.z_ - a.z_*b.y_,
          a.z_*b.x_ - a.x_*b.z_,
          a.x_*b.y_ - a.y_*b.z_};
}

template <typename T>
T tripleProduct(const Vector3D<T>& a,
                const Vector3D<T>& b,
                const Vector3D<T>& c) {
  return dot(a, crossProduct(b, c));
}

}

#endif // TRIANGLES_3D_GEOMETRY_VECTOR_3D_HH_
