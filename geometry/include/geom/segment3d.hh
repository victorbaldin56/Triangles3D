#pragma once

#include "line3d.hh"
#include "range3d.hh"

namespace geometry {

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
struct Segment3D {
  Vector3D<T> begin_;
  Vector3D<T> end_;

  Line3D<T> line() const noexcept { return Line3D<T>(end_ - begin_, begin_); }
  Vector3D<T> getIntersectionPoint(const Line3D<T>& l) const noexcept {
    auto&& this_line = line();
    if (!this_line.valid()) {
      if (l.contains(begin_)) {
        return begin_;
      }
      return Vector3D<T>{};
    }

    auto&& intersection_point = this_line.getIntersectionPoint(l);
    if (getRange().contains(intersection_point)) {
      return intersection_point;
    }
    return Vector3D<T>{};
  }

  bool intersectsOnLine(const Segment3D<T>& other) const noexcept {
    auto&& range = getRange();
    return range.contains(other.begin_) || range.contains(other.end_);
  }

  bool intersects(const Segment3D<T>& other) const noexcept {
    auto copy = *this;
    auto other_copy = other;
    auto&& this_line = copy.line();
    auto&& other_line = other_copy.line();

    if (!other_line.valid()) {
      if (!this_line.valid()) {
        return copy.begin_.isClose(other_copy.begin_);
      }
      std::swap(other_copy, copy);
      std::swap(other_line, this_line);
    }

    if (this_line.isClose(other_line)) {
      return intersectsOnLine(other_copy);
    }

    auto&& intersection = getIntersectionPoint(other_line);
    return other_copy.getRange().contains(intersection);
  }

  /**
   * Returns Range3D built on current segment as on main diagonal.
   */
  Range3D<T> getRange() const noexcept {
    return {std::min(begin_.x_, end_.x_), std::max(begin_.x_, end_.x_),
            std::min(begin_.y_, end_.y_), std::max(begin_.y_, end_.y_),
            std::min(begin_.z_, end_.z_), std::max(begin_.z_, end_.z_)};
  }
};

}  // namespace geometry
