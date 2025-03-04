#pragma once

#include "vector3d.hh"

namespace geometry {

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
class Line3D {
  Vector3D<T> direction_, origin_;

 public:
  Line3D() {}
  Line3D(Vector3D<T> direction, const Vector3D<T>& origin) noexcept
      : direction_(direction.normalize()), origin_(origin) {}

  // getters
 public:
  const Vector3D<T>& direction() const & noexcept { return direction_; }
  Vector3D<T>&& direction() && noexcept { return direction_; }
  const Vector3D<T>& origin() const & noexcept { return origin_; }
  Vector3D<T>&& origin() && noexcept { return origin_; }

 public:
  bool valid() const noexcept { return direction_.valid() && origin_.valid(); }
  bool contains(const Vector3D<T>& p) const noexcept {
    return collinear(p - origin_, direction_);
  }

  Vector3D<T> getIntersectionPoint(const Line3D<T>& other) const noexcept {
    Vector3D<T> dir_cross = crossProduct(direction_, other.direction_);
    Vector3D<T> origin_cross = crossProduct(origin_, other.origin_);
    return origin_ +
           direction_ * dot(dir_cross, origin_cross) / dir_cross.norm2();
  }

  bool isClose(const Line3D<T>& other) const noexcept {
    return collinear(direction_, other.direction_) &&
           collinear(origin_ - other.origin_, direction_);
  }
};

}  // namespace geometry
