#pragma once

#include "vector3d.hh"

namespace geometry {

template <typename T>
class Line3D {
  Vector3D<T> direction_, origin_;

 public:
  Line3D() {}
  Line3D(const Vector3D<T>& direction, const Vector3D<T>& origin) noexcept
      : direction_{direction}, origin_{origin} {
    direction_.normalize();  // same reason as for Plane3D
  }

  // getters
 public:
  const Vector3D<T>& direction() const & noexcept { return direction_; }
  Vector3D<T>&& direction() && noexcept { return direction_; }
  const Vector3D<T>& origin() const & noexcept { return origin_; }
  Vector3D<T>&& origin() && noexcept { return origin_; }

 public:
  bool valid() const noexcept { return direction_.valid() && origin_.valid(); }
  bool contains(const Vector3D<T>& p,
                T abs_tol = comparator::absoluteTolerance<T>(),
                T rel_tol = comparator::relativeTolerance<T>()) const noexcept {
    return collinear(p - origin_, direction_, abs_tol, rel_tol);
  }

  Vector3D<T> getIntersectionPoint(const Line3D<T>& other) const noexcept {
    T triple_product =
        tripleProduct(origin_ - other.origin_, direction_, other.direction_);

    Vector3D<T> dir_cross = crossProduct(direction_, other.direction_);
    Vector3D<T> origin_cross = crossProduct(origin_, other.origin_);
    return origin_ +
           direction_ * dot(dir_cross, origin_cross) / dir_cross.norm2();
  }

  bool isClose(const Line3D<T>& other,
               T abs_tol = comparator::absoluteTolerance<T>(),
               T rel_tol = comparator::relativeTolerance<T>()) const noexcept {
    return collinear(direction_, other.direction_, abs_tol, rel_tol) &&
           collinear(origin_ - other.origin_, direction_, abs_tol, rel_tol);
  }
};

}  // namespace geometry
