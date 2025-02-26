#ifndef TRIANGLES_3D_GEOMETRY_SEGMENT_3D_HH_
#define TRIANGLES_3D_GEOMETRY_SEGMENT_3D_HH_

#include "line3d.hh"
#include "range3d.hh"

namespace geometry {

template <typename T>
struct Segment3D {
  Vector3D<T> begin_;
  Vector3D<T> end_;

  Line3D<T> line() const { return Line3D<T>(end_ - begin_, begin_); }

  bool intersectsLine(const Line3D<T>& line) const {
    Line3D<T> this_line = line();
    Vector3D<T> intersection_point = this_line.getIntersectionPoint(line);

    if (!intersection_point.valid()) {
      return false;
    }

    T x_min = std::min(begin_.x_, end_.x_),
      x_max = std::max(begin_.x_, end_.x_);

    T y_min = std::min(begin_.y_, end_.y_),
      y_max = std::max(begin_.y_, end_.y_);

    T z_min = std::min(begin_.z_, end_.z_),
      z_max = std::max(begin_.z_, end_.z_);

    return x_min <= intersection_point.x_ && intersection_point.x_ <= x_max &&
           y_min <= intersection_point.y_ && intersection_point.y_ <= y_max &&
           z_min <= intersection_point.z_ && intersection_point.z_ <= z_max;
  }

  bool intersects(const Segment3D<T>& other) const {
    return intersectsLine(other.line()) && other.intersectsLine(line());
  }

  Range3D<T> getRange() const {
    return {std::min(begin_.x_, end_.x_), std::max(begin_.x_, end_.x_),
            std::min(begin_.y_, end_.y_), std::max(begin_.y_, end_.y_),
            std::min(begin_.z_, end_.z_), std::max(begin_.z_, end_.z_)};
  }
};

}

#endif // TRIANGLES_3D_GEOMETRY_SEGMENT_3D_HH_
