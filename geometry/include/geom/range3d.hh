#pragma once

#include "vector3d.hh"

namespace geometry {

template <typename T>
struct Range3D {
  T min_x, max_x;
  T min_y, max_y;
  T min_z, max_z;

  bool contains(const Vector3D<T>& p) {
    return comparator::isLess(min_x, p.x_) && comparator::isLess(p.x_, max_x) &&
           comparator::isLess(min_y, p.y_) && comparator::isLess(p.y_, max_y) &&
           comparator::isLess(min_z, p.z_) && comparator::isLess(p.z_, max_z);
  }
};

} // namespace geometry
