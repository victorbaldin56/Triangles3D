#ifndef TRIANGLES_3D_GEOMETRY_TRIANGLE_3D_HH_
#define TRIANGLES_3D_GEOMETRY_TRIANGLE_3D_HH_

#include "plane.hh"
#include "segment3d.hh"

namespace geometry {

template <typename T>
struct Triangle3D {
  Vector3D<T> a_, b_, c_;

  bool intersectsLine(const Line3D<T>& line) const {
    Segment3D<T> ab = {a_, b_};
    Segment3D<T> bc = {b_, c_};
    Segment3D<T> ac = {a_, c_};
    return ab.intersectsLine(line) ||
           bc.intersectsLine(line) ||
           ac.intersectsLine(line);
  }

  T area() const {
    return norm(crossProduct(a_ - b_, a_ - c_))/2;
  }

  bool inside(const Vector3D<T>& point) const {
    Triangle3D<T> t1 = {a_, b_, point};
    Triangle3D<T> t2 = {b_, c_, point};
    Triangle3D<T> t3 = {a_, c_, point};
    return numeric::equal(area(), t1.area() + t2.area() + t3.area());
  }

  bool intersectsInPlane(const Triangle3D<T>& other) const {
    return inside(other.a_) || inside(other.b_) || inside(other.c_);
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
    return intersectsLine(intersection_line) &&
           other.intersectsLine(intersection_line);
  }
};

}

#endif // TRIANGLES_3D_GEOMETRY_TRIANGLE_3D_HH_
