#pragma once

#include "vector3d.hh"

namespace geometry {

template <typename T>
struct Range3D {
  T min_x, max_x;  // min_x >= max_x means empty range
  T min_y, max_y;  // min_y >= max_y means empty range
  T min_z, max_z;  // min_z >= max_z means empty range

  bool contains(const Vector3D<T>& p) {
    return comparator::isLessClose(min_x, p.x_) &&
           comparator::isLessClose(p.x_, max_x) &&
           comparator::isLessClose(min_y, p.y_) &&
           comparator::isLessClose(p.y_, max_y) &&
           comparator::isLessClose(min_z, p.z_) &&
           comparator::isLessClose(p.z_, max_z);
  }
};

}  // namespace geometry
