#ifndef TRIANGLES_3D_GEOMETRY_LINE_3D_HH_
#define TRIANGLES_3D_GEOMETRY_LINE_3D_HH_

#include "vector3d.hh"

namespace geometry {

template <typename T>
class Line3D {
  Vector3D<T> direction, origin_;

 public:
  Line3D() {}
};

}

#endif // TRIANGLES_3D_GEOMETRY_LINE_3D_HH_
