#ifndef TRIANGLES_3D_GEOMETRY_TRIANGLE_3D_HH_
#define TRIANGLES_3D_GEOMETRY_TRIANGLE_3D_HH_

#include <algorithm>

#include "plane.hh"
#include "segment3d.hh"

namespace geometry {

template <typename T>
struct Triangle3D {
  Vector3D<T> a_, b_, c_;

  using NumType = T; // for interface

  bool intersectsLine(const Line3D<T>& line) const {
    Segment3D<T> ab = {a_, b_};
    Segment3D<T> bc = {b_, c_};
    Segment3D<T> ac = {a_, c_};
    return ab.intersectsLine(line) ||
           bc.intersectsLine(line) ||
           ac.intersectsLine(line);
  }

  T area() const {
    return crossProduct(a_ - b_, a_ - c_).norm()/2;
  }

  T minX() const { return std::min({a_.x_, b_.x_, c_.x_}); }
  T maxX() const { return std::max({a_.x_, b_.x_, c_.x_}); }
  T minY() const { return std::min({a_.y_, b_.y_, c_.y_}); }
  T maxY() const { return std::max({a_.y_, b_.y_, c_.y_}); }
  T minZ() const { return std::min({a_.z_, b_.z_, c_.z_}); }
  T maxZ() const { return std::max({a_.z_, b_.z_, c_.z_}); }

  bool inside(const Vector3D<T>& point) const {
    Triangle3D<T> t1 = {a_, b_, point};
    Triangle3D<T> t2 = {b_, c_, point};
    Triangle3D<T> t3 = {a_, c_, point};
    return comparator::isClose(area(), t1.area() + t2.area() + t3.area());
  }

  bool intersectsInPlane(const Triangle3D<T>& other) const {
    return inside(other.a_) || inside(other.b_) || inside(other.c_);
  }

  bool intersects(const Triangle3D<T>& other) const {
    Plane<T> this_p{a_, b_, c_};
    Plane<T> other_p{other.a_, other.b_, other.c_};

    // planes are coincident with doubleing point tolerance
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

template <typename T>
std::istream& operator>>(std::istream& is, Triangle3D<T>& t) {
  is >> t.a_ >> t.b_ >> t.c_;
  return is;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Triangle3D<T>& t) {
  os << t.a_ << t.b_ << t.c_;
  return os;
}

}

#endif // TRIANGLES_3D_GEOMETRY_TRIANGLE_3D_HH_
