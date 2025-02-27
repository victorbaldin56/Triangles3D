#pragma once

#include "vector3d.hh"
#include "line3d.hh"
#include "segment3d.hh"

namespace geometry {

template <typename T>
class Plane {
  Vector3D<T> n_;
  T d_;

  // constructors
 public:
  Plane() {}
  Plane(const Vector3D<T>& a, const Vector3D<T>& b, const Vector3D<T>& c)
    : n_{crossProduct(a - b, a - c).normalize()}, d_{dot(n_, a)} {}

  // getters
 public:
  const Vector3D<T>& normal() const & {
    return n_;
  }

  Vector3D<T>&& normal() && {
    return n_;
  }

  T distance() const {
    return d_;
  }

 public:
  bool valid() const {
    return n_.valid() && std::isfinite(d_);
  }

  Line3D<T> getIntersectionLine(const Plane<T>& other) const {
    T normal_dot = dot(n_, other.n_);
    T denom = 1 - normal_dot*normal_dot;
    T c1 = (d_ - other.d_*normal_dot)/denom;
    T c2 = (other.d_ - d_*normal_dot)/denom;

    return Line3D<T>{crossProduct(n_, other.n_), c1*n_ + c2*other.n_};
  }

  Vector3D<T> getIntersectionPoint(const Line3D<T>& line) const {
    return
        line.origin()
            + (d_ - dot(line.origin(), n_) / dot(line.direction(), n_))
                * line.direction();
  }

  Vector3D<T> getIntersectionPoint(const Segment3D<T>& seg) const {
    Line3D<T> l = seg.line();
    auto p = getIntersectionPoint(l);
    auto range = seg.getRange();
    if (range.contains(p)) {
      return p;
    }
    return Vector3D<T>();
  }

  bool equal(const Plane<T>& other) const {
    return (n_.equal(other.n_) && comparator::isClose(d_, other.d_)) ||
           (n_.equal(-other.n_) && comparator::isClose(d_, -other.d_));
  }
};

}

