#ifndef TRIANGLES_3D_GEOMETRY_TRIANGLE_3D_HH_
#define TRIANGLES_3D_GEOMETRY_TRIANGLE_3D_HH_

#include "plane.hh"

namespace geometry {

template <typename T>
struct Triangle3D {
  Vector3D<T> a_, b_, c_;

  bool intersects(const Line3D<T>& line) const {

  }

  bool intersects(const Triangle3D<T>& other) const {
    Plane<T> this_p{a_, b_, c_};
    Plane<T> other_p{other.a_, other.b_, other.c_};

    // planes are coincident with floating point tolerance
    if (this_p.equal(other_p)) {
      return intersectsInPlane(other);
    }

    Line3D<T> intersection_line = this_p.getIntersectionLine(other_p);

    // planes are parallel
    if (!intersection_line.valid()) {
      return false;
    }

    // main case: planes have intersection line

  }

 private:
  bool intersectsInPlane(const Triangle3D<T>& other) const {

  }
};

}

#endif // TRIANGLES_3D_GEOMETRY_TRIANGLE_3D_HH_
