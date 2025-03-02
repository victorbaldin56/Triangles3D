#pragma once

#include <cassert>

#include "spdlog/spdlog.h"

#include "line3d.hh"
#include "segment3d.hh"
#include "vector3d.hh"

namespace geometry {

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
class Plane {
  Vector3D<T> n_;
  T d_;

  // constructors
 public:
  Plane() noexcept {}
  Plane(const Vector3D<T>& a,
        const Vector3D<T>& b,
        const Vector3D<T>& c) noexcept
      : n_(crossProduct(a - b, a - c).normalize()),
        // normalization applied here to make easier comparison between planes
        // and make plane with zero normal vector invalid in place
        d_(dot(n_, a)) {
    assert(!valid() || contains(a) && contains(b) && contains(c));
  }

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
    assert(comparator::isClose(n_.norm(), static_cast<T>(1)));

    auto&& origin = line.origin();
    auto&& dir = line.direction();
    SPDLOG_TRACE("n_.x_ = {}, n_.y_ = {}, n_.z_ = {}",
                 n_.x_, n_.y_, n_.z_);
    SPDLOG_TRACE("origin.x_ = {}, origin.y_ = {}, origin.z_ = {}",
                 origin.x_, origin.y_, origin.z_);
    SPDLOG_TRACE("dir.x_ = {}, dir.y_ = {}, dir.z_ = {}",
                 dir.x_, dir.y_, dir.z_);
    auto&& denominator = dot(n_, dir);
    if (comparator::isClose(denominator, static_cast<T>(0))) {
      // check if the line lies on the plane
      if (comparator::isClose(dot(n_, origin), d_)) {
        return origin;  // any point on the line is valid, return the origin
      }
      return Vector3D<T>{};  // no intersection
    }

    auto&& res = origin + dir * (d_ - dot(origin, n_) / denominator);
    assert(line.contains(res));
    assert(contains(res));
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
    assert(!p.valid() || contains(p));
    assert(!p.valid() || l.contains(p));

    auto range = seg.getRange();
    if (range.contains(p)) {
      return p;
    }
    return Vector3D<T>();
  }

  bool contains(const Vector3D<T>& p) const noexcept {
    auto&& dotpn = dot(p, n_);
    SPDLOG_TRACE("Plane3D::contains(p.x_ = {}, p.y_ = {}, p.z_ = {})",
                 p.x_, p.y_, p.z_);
    SPDLOG_TRACE("dotpn = {}, d_ = {}", dotpn, d_);
    return comparator::isClose(dotpn, d_);
  }

  bool isClose(const Plane<T>& other) const noexcept {
    return (n_.isClose(other.n_) &&
            comparator::isClose(d_, other.d_)) ||
           (n_.isClose(-other.n_) &&
            comparator::isClose(d_, -other.d_));
  }
};

}  // namespace geometry
