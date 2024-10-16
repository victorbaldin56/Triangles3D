#ifndef TRIANGLES_3D_GEOMETRY_SEGMENT_3D_HH_
#define TRIANGLES_3D_GEOMETRY_SEGMENT_3D_HH_

#include "line3d.hh"

namespace geometry {

template <typename T>
struct Segment3D {
  Vector3D<T> begin_;
  Vector3D<T> end_;

  bool intersectsLine(const Line3D<T>& line) const {
    Line3D<T> this_line{end_ - begin_, begin_};
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
};

}

#endif // TRIANGLES_3D_GEOMETRY_SEGMENT_3D_HH_
