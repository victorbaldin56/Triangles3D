#ifndef TRIANGLES_3D_GEOMETRY_LINE_3D_HH_
#define TRIANGLES_3D_GEOMETRY_LINE_3D_HH_

#include "vector3d.hh"

namespace geometry {

template <typename T>
class Line3D {
  Vector3D<T> direction_, origin_;

 public:
  Line3D() {}
  Line3D(const Vector3D<T>& direction, const Vector3D<T> origin)
    : direction_{direction}, origin_{origin} {}

  // getters
 public:
  const Vector3D<T>& direction() const & {
    return direction_;
  }

  Vector3D<T>&& direction() && {
    return direction_;
  }

  const Vector3D<T>& origin() const & {
    return origin_;
  }

  Vector3D<T>&& origin() && {
    return origin_;
  }

 public:
  bool valid() const {
    return direction_.valid() && origin_.valid();
  }

  Vector3D<T>& getIntersectionPoint(const Line3D<T>& other) const {

  }

  bool equal(const Line3D<T>& other) {
    return direction_.equal(other.direction_) && origin_.equal(other.origin_);
  }
};

}

#endif // TRIANGLES_3D_GEOMETRY_LINE_3D_HH_
