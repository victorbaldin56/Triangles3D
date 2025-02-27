#pragma once

#include "vector3d.hh"

namespace geometry {

template <typename T>
struct Range3D {
  T min_x_, max_x_;  // min_x >= max_x means empty range
  T min_y_, max_y_;  // min_y >= max_y means empty range
  T min_z_, max_z_;  // min_z >= max_z means empty range

  bool contains(
      const Vector3D<T>& p,
      T abs_tol = comparator::absoluteTolerance<T>(),
      T rel_tol = comparator::relativeTolerance<T>()) const noexcept {
    return comparator::isLessClose(min_x_, p.x_, abs_tol, rel_tol) &&
           comparator::isLessClose(p.x_, max_x_, abs_tol, rel_tol) &&
           comparator::isLessClose(min_y_, p.y_, abs_tol, rel_tol) &&
           comparator::isLessClose(p.y_, max_y_, abs_tol, rel_tol) &&
           comparator::isLessClose(min_z_, p.z_, abs_tol, rel_tol) &&
           comparator::isLessClose(p.z_, max_z_, abs_tol, rel_tol);
  }

  bool contains(
      const Range3D<T>& other,
      T abs_tol = comparator::absoluteTolerance<T>(),
      T rel_rol = comparator::relativeTolerance<T>()) const noexcept {
    return comparator::isLessClose(min_x_, other.min_x_, abs_tol, rel_rol) &&
           comparator::isLessClose(other.max_x_, max_x_, abs_tol, rel_rol) &&
           comparator::isLessClose(min_y_, other.min_y_, abs_tol, rel_rol) &&
           comparator::isLessClose(other.max_y_, max_y_, abs_tol, rel_rol) &&
           comparator::isLessClose(min_z_, other.min_z_, abs_tol, rel_rol) &&
           comparator::isLessClose(other.max_z_, max_z_, abs_tol, rel_rol);
  }

  T dimX() const noexcept { return max_x_ - min_x_; }
  T dimY() const noexcept { return max_y_ - min_y_; }
  T dimZ() const noexcept { return max_z_ - min_z_; }
};

}  // namespace geometry
