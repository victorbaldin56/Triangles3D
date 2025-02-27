#pragma once

#include "line3d.hh"
#include "range3d.hh"

namespace geometry {

template <typename T>
struct Segment3D {
  Vector3D<T> begin_;
  Vector3D<T> end_;

  Line3D<T> line() const { return Line3D<T>(end_ - begin_, begin_); }

  bool intersectsLine(const Line3D<T>& l) const {
    Line3D<T> this_line = line();
    Vector3D<T> intersection_point = this_line.getIntersectionPoint(l);

    if (!intersection_point.valid()) {
      return false;
    }
    return getRange().contains(intersection_point);
  }

  bool intersects(const Segment3D<T>& other) const {
    return intersectsLine(other.line()) && other.intersectsLine(line());
  }

  /**
   * Returns Range3D built on current segment as on main diagonal.
   */
  Range3D<T> getRange() const {
    return {std::min(begin_.x_, end_.x_), std::max(begin_.x_, end_.x_),
            std::min(begin_.y_, end_.y_), std::max(begin_.y_, end_.y_),
            std::min(begin_.z_, end_.z_), std::max(begin_.z_, end_.z_)};
  }
};

}  // namespace geometry
