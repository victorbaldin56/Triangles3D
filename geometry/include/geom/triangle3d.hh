#ifndef TRIANGLES_3D_GEOMETRY_TRIANGLE_3D_HH_
#define TRIANGLES_3D_GEOMETRY_TRIANGLE_3D_HH_

#include <algorithm>

#include "plane.hh"
#include "segment3d.hh"

namespace geometry {

template <typename T>
struct Triangle3D {
  Vector3D<T> a_, b_, c_;

  Segment3D<T> toSegment() const {
    auto max_x = std::max({a_.x_, b_.x_, c_.x_});
    auto min_x = std::min({a_.x_, b_.x_, c_.x_});

    if (a_.x_ != max_x && a_.x_ != min_x) {
      return {b_, c_};
    }
    if (b_.x_ != max_x && b_.x_ != min_x) {
      return {a_, c_};
    }
    if (c_.x_ != max_x && c_.x_ != min_x) {
      return {a_, b_};
    }
  }

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

  bool contains(const Vector3D<T>& point) const {
    Triangle3D<T> t1 = {a_, b_, point};
    Triangle3D<T> t2 = {b_, c_, point};
    Triangle3D<T> t3 = {a_, c_, point};
    return comparator::isClose(area(), t1.area() + t2.area() + t3.area());
  }

  bool intersectsInPlane(const Triangle3D<T>& other) const {
    return contains(other.a_) || contains(other.b_) || contains(other.c_);
  }

  bool intersects(const Triangle3D<T>& other) const {
    auto copy(*this);
    auto other_copy(other);
    Plane<T> this_p{copy.a_, copy.b_, copy.c_};
    Plane<T> other_p{other_copy.a_, other_copy.b_, other_copy.c_};

    if (!other_p.valid()) {
      if (!this_p.valid()) {
        Segment3D<T> this_seg = copy.toSegment();
        Segment3D<T> other_seg = other_copy.toSegment();
        return this_seg.intersects(other_seg);
      }
      std::swap(other_copy, copy);
      std::swap(other_p, this_p);
    }

    // planes are coincident with floating point tolerance
    if (this_p.equal(other_p)) {
      return copy.intersectsInPlane(other);
    }

    Segment3D<T> ab = {copy.a_, copy.b_};
    Segment3D<T> bc = {copy.b_, copy.c_};
    Segment3D<T> ac = {copy.a_, copy.c_};

    Vector3D<T> abi = other_p.getIntersectionPoint(ab);
    Vector3D<T> bci = other_p.getIntersectionPoint(bc);
    Vector3D<T> aci = other_p.getIntersectionPoint(ac);

    return other_copy.contains(abi)
           || other_copy.contains(bci)
           || other_copy.contains(aci);
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
