#pragma once

#include <cassert>

#include "line3d.hh"
#include "segment3d.hh"
#include "vector3d.hh"

namespace geometry {

template <typename T>
class Plane final {
  Vector3D<T> n_;
  T d_;

  // constructors
 public:
  Plane() noexcept {}
  Plane(const Vector3D<T>& a, const Vector3D<T>& b,
        const Vector3D<T>& c) noexcept
      : n_(crossProduct(a - b, a - c).normalize()),
        // normalization applied here to make easier comparison between planes
        // and make plane with zero normal vector invalid in place
        d_(dot(n_, a)) {}

  // getters
 public:
  const Vector3D<T>& normal() const noexcept { return n_; }
  T distance() const noexcept { return d_; }

 public:
  bool valid() const noexcept { return n_.valid() && std::isfinite(d_); }

  Line3D<T> getIntersectionLine(const Plane<T>& other) const noexcept {
    auto normal_dot = dot(n_, other.n_);
    auto denom = 1 - normal_dot * normal_dot;
    auto c1 = (d_ - other.d_ * normal_dot) / denom;
    auto c2 = (other.d_ - d_ * normal_dot) / denom;

    return Line3D<T>(crossProduct(n_, other.n_), c1 * n_ + c2 * other.n_);
  }

  Vector3D<T> getIntersectionPoint(const Line3D<T>& line) const noexcept {
    auto&& origin = line.origin();
    auto&& dir = line.direction();
    auto denominator = dot(n_, dir);
    if (comparator::isClose(denominator, static_cast<T>(0))) {
      // check if the line lies on the plane
      return Vector3D<T>{};  // no intersection
    }

    auto t = (d_ - dot(origin, n_)) / denominator;
    auto res = origin + dir * t;
    return res;
  }

  Vector3D<T> getIntersectionPoint(const Segment3D<T>& seg) const noexcept {
    Line3D<T> l = seg.line();
    if (!l.valid()) {
      if (contains(seg.begin_)) {
        return seg.begin_;  // segment decays to point
      }
      return Vector3D<T>{};
    }
    auto p = getIntersectionPoint(l);
    auto range = seg.getRange();
    if (range.contains(p)) {
      return p;
    }
    return Vector3D<T>{};
  }

  bool contains(const Vector3D<T>& p) const noexcept {
    auto dotpn = dot(p, n_);
    return comparator::isClose(dotpn, d_);
  }

  bool contains(const Segment3D<T>& s) const noexcept {
    return contains(s.begin_) && contains(s.end_);
  }

  bool isClose(const Plane<T>& other) const noexcept {
    return (n_.isClose(other.n_) && comparator::isClose(d_, other.d_)) ||
           (n_.isClose(-other.n_) && comparator::isClose(d_, -other.d_));
  }
};

}  // namespace geometry
