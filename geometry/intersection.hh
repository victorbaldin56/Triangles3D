#ifndef TRIANGLES_3D_GEOMETRY_INTERSECTION_HH_
#define TRIANGLES_3D_GEOMETRY_INTERSECTION_HH_

#include "plane.hh"
#include "line3d.hh"

namespace geometry {

template <typename F1, typename F2>
struct Intersection;

template <typename T>
struct Intersection<Plane<T>, Plane<T>> {
  Line3D<T> line_;

  enum TypeTag {
    kEmpty,
    kLine,
    kPlane,
  } type_tag_;
};

}

#endif // TRIANGLES_3D_GEOMETRY_INTERSECTION_HH_
