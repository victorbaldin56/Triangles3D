#pragma once

#include "line3d.hh"
#include "segment3d.hh"
#include "vector3d.hh"

namespace geometry {

template <typename T>
class Plane {
  Vector3D<T> n_;
  T d_;

  // constructors
 public:
  Plane() noexcept {}
  Plane(const Vector3D<T>& a,
        const Vector3D<T>& b,
        const Vector3D<T>& c) noexcept
      : n_{crossProduct(a - b, a - c).normalize()},
        // normalization applied here to make easier comparison between planes
        // and make plane with zero normal vector invalid in place
        d_{dot(n_, a)} {}

  // getters
 public:
  const Vector3D<T>& normal() const & noexcept { return n_; }
  Vector3D<T>&& normal() && noexcept { return n_; }
  T distance() const noexcept { return d_; }

 public:
  bool valid() const noexcept { return n_.valid() && std::isfinite(d_); }

  Line3D<T> getIntersectionLine(const Plane<T>& other) const noexcept {
    T normal_dot = dot(n_, other.n_);
    T denom = 1 - normal_dot * normal_dot;
    T c1 = (d_ - other.d_ * normal_dot) / denom;
    T c2 = (other.d_ - d_ * normal_dot) / denom;

    return Line3D<T>{crossProduct(n_, other.n_), c1 * n_ + c2 * other.n_};
  }

  Vector3D<T> getIntersectionPoint(const Line3D<T>& line) const noexcept {
    return line.origin() +
           (d_ - dot(line.origin(), n_) / dot(line.direction(), n_)) *
               line.direction();
  }

  Vector3D<T> getIntersectionPoint(
      const Segment3D<T>& seg,
      T abs_tol = comparator::absTolerance<T>(),
      T rel_tol = comparator::relTolerance<T>()) const noexcept {
    Line3D<T> l = seg.line();
    if (!l.valid()) {
      if (contains(seg.begin_)) {
        return seg.begin_;  // segment decays to point
      }
      return Vector3D<T>{};
    }
    auto p = getIntersectionPoint(l);
    auto range = seg.getRange();
    if (range.contains(p, abs_tol, rel_tol)) {
      return p;
    }
    return Vector3D<T>();
  }

  bool contains(const Vector3D<T>& p,
                T abs_tol = comparator::absTolerance<T>(),
                T rel_tol = comparator::relTolerance<T>()) const noexcept {
    return comparator::isClose(dot(p, n_), d_, abs_tol, rel_tol);
  }

  bool isClose(const Plane<T>& other,
               T abs_tol = comparator::absTolerance<T>(),
               T rel_tol = comparator::relTolerance<T>()) const noexcept {
    return (n_.isClose(other.n_, abs_tol, rel_tol) &&
            comparator::isClose(d_, other.d_, abs_tol, rel_tol)) ||
           (n_.isClose(-other.n_, abs_tol, rel_tol) &&
            comparator::isClose(d_, -other.d_, abs_tol, rel_tol));
  }
};

}  // namespace geometry
