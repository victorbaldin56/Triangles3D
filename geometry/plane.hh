#ifndef TRIANGLES_3D_GEOMETRY_PLANE_HH_
#define TRIANGLES_3D_GEOMETRY_PLANE_HH_

#include "vector3d.hh"

namespace geometry {

template <typename T>
class Plane {
  Vector3D<T> n_;
  T           d_;

  // constructors
 public:
  Plane() {}
  Plane(const Vector3D<T>& a, const Vector3D<T>& b, const Vector3D<T>& c)
    : n_{crossProduct(a - b, a - c).normalize()}, d_{dot(n_, a)} {}

  // getters
 public:
  Vector3D<T>& normal() & {
    return n_;
  }

  const Vector3D<T>& normal() const & {
    return n_;
  }

  Vector3D<T>&& normal() && {
    return n_;
  }

  T distance() const {
    return d_;
  }
};

}

#endif // TRIANGLES_3D_GEOMETRY_PLANE_HH_
