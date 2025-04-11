#pragma once

#include <algorithm>
#include <set>
#include <vector>

#include "geom/triangle3d.hh"
#include "glhpp/vertex_array.hh"

namespace triangles_gl {

class Scene final {
 public:
  Scene(const std::vector<geometry::Triangle3D<float>>& triangles,
        const std::set<std::size_t> red_indices) {
    auto triangles_count = triangles.size();
    vertices_.reserve(triangles_count * 3);
    for (std::size_t i = 0; i < triangles_count; ++i) {
      GLbyte color = red_indices.count(i);
      auto&& triangle = triangles[i];
      auto normal = triangle.normal();
      vertices_.push_back({triangle.a_, normal, color});
      vertices_.push_back({triangle.b_, normal, color});
      vertices_.push_back({triangle.c_, normal, color});
    }
  }

  const auto& getVertices() const noexcept { return vertices_; }

 private:
  std::vector<glhpp::Vertex> vertices_;
};
}
