#pragma once

#include "vector3d.hh"

namespace geometry {

template <typename T>
struct Range3D {
  T min_x, max_x;  // min_x >= max_x means empty range
  T min_y, max_y;  // min_y >= max_y means empty range
  T min_z, max_z;  // min_z >= max_z means empty range

  bool contains(const Vector3D<T>& p,
                T abs_tol = comparator::absTolerance<T>(),
                T rel_tol = comparator::relTolerance<T>()) const noexcept {
    return comparator::isLessClose(min_x, p.x_, abs_tol, rel_tol) &&
           comparator::isLessClose(p.x_, max_x, abs_tol, rel_tol) &&
           comparator::isLessClose(min_y, p.y_, abs_tol, rel_tol) &&
           comparator::isLessClose(p.y_, max_y, abs_tol, rel_tol) &&
           comparator::isLessClose(min_z, p.z_, abs_tol, rel_tol) &&
           comparator::isLessClose(p.z_, max_z, abs_tol, rel_tol);
  }
};

}  // namespace geometry
