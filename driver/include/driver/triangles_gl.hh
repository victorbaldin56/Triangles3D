#pragma once

#include <vector>
#include <set>

#include "geom/triangle3d.hh"

namespace gl_app {

class Triangles final {
 public:
  using Triangle = geometry::Triangle3D<float>;

  Triangles(const std::vector<Triangle>& triangles,
            const std::set<std::size_t>& red);
};

}
