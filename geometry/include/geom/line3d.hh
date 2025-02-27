#pragma once

#include "vector3d.hh"

namespace geometry {

template <typename T>
class Line3D {
  Vector3D<T> direction_, origin_;

 public:
  Line3D() {}
  Line3D(const Vector3D<T>& direction, const Vector3D<T>& origin)
      : direction_{direction}, origin_{origin} {
    direction_.normalize();  // same reason as for Plane3D
  }

  // getters
 public:
  const Vector3D<T>& direction() const& { return direction_; }
  Vector3D<T>&& direction() && { return direction_; }
  const Vector3D<T>& origin() const& { return origin_; }
  Vector3D<T>&& origin() && { return origin_; }

 public:
  bool valid() const { return direction_.valid() && origin_.valid(); }

  Vector3D<T> getIntersectionPoint(const Line3D<T>& other) const {
    T triple_product =
        tripleProduct(origin_ - other.origin_, direction_, other.direction_);

    // skew lines
    if (!comparator::isClose(triple_product, static_cast<T>(0))) {
      return Vector3D<T>{};
    }

    Vector3D<T> dir_cross = crossProduct(direction_, other.direction_);
    Vector3D<T> origin_cross = crossProduct(origin_, other.origin_);
    return origin_ +
           direction_ * dot(dir_cross, origin_cross) / dir_cross.norm2();
  }

  bool equal(const Line3D<T>& other) {
    return collinear(direction_, other.direction_) &&
           collinear(origin_ - other.origin_, direction_);
  }
};

}  // namespace geometry
