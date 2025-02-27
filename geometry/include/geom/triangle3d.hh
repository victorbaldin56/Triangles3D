#pragma once

#include <algorithm>

#include "plane.hh"
#include "segment3d.hh"

namespace geometry {

template <typename T>
struct Triangle3D {
  Vector3D<T> a_, b_, c_;

  /**
   * Decays degenerate triangle to segment.
   */
  Segment3D<T> toSegment() const {
    auto ab = Segment3D<T>{a_, b_};
    auto bc = Segment3D<T>{b_, c_};
    auto ac = Segment3D<T>{a_, c_};

    if (ab.getRange().contains(c_)) {
      return ab;
    }
    if (bc.getRange().contains(a_)) {
      return bc;
    }
    return ac;
  }

  bool isDegenerate() const noexcept { return Plane<T>(a_, b_, c_).valid(); }
  T area() const { return crossProduct(a_ - b_, a_ - c_).norm() / 2; }

  /**
   * Returns a minimal range containing the whole triangle.
   */
  Range3D<T> getRange() const noexcept {
    auto xs = {a_.x_, b_.x_, c_.x_};
    auto ys = {a_.y_, b_.y_, c_.y_};
    auto zs = {a_.z_, b_.z_, c_.z_};
    return {std::min(xs), std::max(xs),
            std::min(ys), std::max(ys),
            std::min(zs), std::max(zs)};
  }

  bool contains(const Vector3D<T>& point,
                T abs_tol = comparator::absoluteTolerance<T>(),
                T rel_tol = comparator::relativeTolerance<T>()) const noexcept {
    Triangle3D<T> t1 = {a_, b_, point};
    Triangle3D<T> t2 = {b_, c_, point};
    Triangle3D<T> t3 = {a_, c_, point};
    return comparator::isClose(
        area(), t1.area() + t2.area() + t3.area(), abs_tol, rel_tol);
  }

  bool intersectsInPlane(const Triangle3D<T>& other) const noexcept {
    return contains(other.a_) ||
           contains(other.b_) ||
           contains(other.c_);
  }

  bool intersects(const Triangle3D<T>& other) const noexcept {
    auto copy(*this);
    auto other_copy(other);
    auto this_p = Plane<T>(copy.a_, copy.b_, copy.c_);
    auto other_p = Plane<T>(other_copy.a_, other_copy.b_, other_copy.c_);

    if (!other_p.valid()) {
      if (!this_p.valid()) {
        auto this_seg = copy.toSegment();
        auto other_seg = other_copy.toSegment();
        return this_seg.intersects(other_seg);
      }
      std::swap(other_copy, copy);
      std::swap(other_p, this_p);
    }

    // planes are coincident with floating point tolerance
    if (this_p.isClose(other_p)) {
      return copy.intersectsInPlane(other);
    }

    auto ab = Segment3D<T>{copy.a_, copy.b_};
    auto bc = Segment3D<T>{copy.b_, copy.c_};
    auto ac = Segment3D<T>{copy.a_, copy.c_};

    auto abi = other_p.getIntersectionPoint(ab);
    auto bci = other_p.getIntersectionPoint(bc);
    auto aci = other_p.getIntersectionPoint(ac);

    return other_copy.contains(abi) ||
           other_copy.contains(bci) ||
           other_copy.contains(aci);
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

}  // namespace geometry
