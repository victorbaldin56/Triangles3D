#pragma once

#include "line3d.hh"
#include "range3d.hh"

namespace geometry {

template <typename T>
struct Segment3D {
  Vector3D<T> begin_;
  Vector3D<T> end_;

  Line3D<T> line() const noexcept { return Line3D<T>(end_ - begin_, begin_); }
  Vector3D<T> getIntersectionPoint(
      const Line3D<T>& l,
      T abs_tol = comparator::absoluteTolerance<T>(),
      T rel_tol = comparator::relativeTolerance<T>()) const noexcept {
    auto this_line = line();
    if (!this_line.valid()) {
      if (l.contains(begin_)) {
        return begin_;
      }
      return Vector3D<T>{};
    }

    auto intersection_point = this_line.getIntersectionPoint(l);
    if (getRange().contains(intersection_point, abs_tol, rel_tol)) {
      return intersection_point;
    }
    return Vector3D<T>{};
  }

  bool intersectsOnLine(
      const Segment3D<T>& other,
      T abs_tol = comparator::absoluteTolerance<T>(),
      T rel_tol = comparator::relativeTolerance<T>()) const noexcept {
    auto range = getRange();
    return range.contains(other.begin_, abs_tol, rel_tol) ||
           range.contains(other.end_, abs_tol, rel_tol);
  }

  bool intersects(const Segment3D<T>& other,
                  T abs_tol = comparator::absoluteTolerance<T>(),
                  T rel_tol = comparator::relativeTolerance<T>()) const noexcept {
    auto copy(*this);
    auto other_copy(other);
    auto this_line = copy.line();
    auto other_line = other_copy.line();

    if (!other_line.valid()) {
      if (!this_line.valid()) {
        return copy.begin_.isClose(other_copy.begin_, abs_tol, rel_tol);
      }
      std::swap(other_copy, copy);
      std::swap(other_line, this_line);
    }

    if (this_line.isClose(other_line, abs_tol, rel_tol)) {
      return intersectsOnLine(other_copy, abs_tol, rel_tol);
    }

    auto intersection = getIntersectionPoint(other_line, abs_tol, rel_tol);
    return other_copy.getRange().contains(intersection, abs_tol, rel_tol);
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
