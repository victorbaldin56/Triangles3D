#pragma once

#include <algorithm>

#include "plane.hh"
#include "segment3d.hh"

namespace geometry {

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
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

  bool contains(const Vector3D<T>& point) const noexcept {
    Triangle3D<T> t1 = {a_, b_, point};
    Triangle3D<T> t2 = {b_, c_, point};
    Triangle3D<T> t3 = {a_, c_, point};
    return comparator::isClose(area(), t1.area() + t2.area() + t3.area());
  }

  bool intersectsInPlane(const Triangle3D<T>& other) const noexcept {
    return contains(other.a_) || contains(other.b_) || contains(other.c_) ||
           other.contains(a_) || other.contains(b_) || other.contains(c_);
  }

  bool intersects(const Triangle3D<T>& other) const noexcept {
    auto this_p = Plane<T>(a_, b_, c_);
    auto other_p = Plane<T>(other.a_, other.b_, other.c_);

    auto this_valid = this_p.valid();
    auto other_valid = other_p.valid();

    if (!this_valid && !other_valid) {
      auto this_seg = toSegment();
      auto other_seg = other.toSegment();
      return this_seg.intersects(other_seg);
    }

    if (this_valid && !other_valid) {
      auto intersection = this_p.getIntersectionPoint(other.toSegment());
      return contains(intersection);
    }

    if (!this_valid && other_valid) {
      auto intersection = other_p.getIntersectionPoint(toSegment());
      return other.contains(intersection);
    }

    // planes are coincident with floating point tolerance
    if (this_p.isClose(other_p)) {
      return intersectsInPlane(other);
    }

    auto ab = Segment3D<T>{a_, b_};
    auto bc = Segment3D<T>{b_, c_};
    auto ac = Segment3D<T>{a_, c_};

    auto oab = Segment3D<T>{other.a_, other.b_};
    auto obc = Segment3D<T>{other.b_, other.c_};
    auto oac = Segment3D<T>{other.a_, other.c_};

    auto abi = other_p.getIntersectionPoint(ab);
    auto bci = other_p.getIntersectionPoint(bc);
    auto aci = other_p.getIntersectionPoint(ac);

    auto oabi = this_p.getIntersectionPoint(oab);
    auto obci = this_p.getIntersectionPoint(obc);
    auto oaci = this_p.getIntersectionPoint(oac);

    return other.contains(abi) || other.contains(bci) || other.contains(aci) ||
           contains(oabi) || contains(obci) || contains(oaci);
  }
};

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
std::istream& operator>>(std::istream& is, Triangle3D<T>& t) {
  is >> t.a_ >> t.b_ >> t.c_;
  return is;
}

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
std::ostream& operator<<(std::ostream& os, const Triangle3D<T>& t) {
  os << t.a_ << t.b_ << t.c_;
  return os;
}

}  // namespace geometry
