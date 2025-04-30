#pragma once

#include <algorithm>
#include <set>
#include <vector>

#include "geom/triangle3d.hh"
#include "glhpp/light.hh"
#include "glhpp/renderer.hh"
#include "glhpp/vertex_array.hh"
#include "vertex.hh"

namespace triangles_gl {

class Scene final {
 public:
  Scene(const std::vector<geometry::Triangle3D<float>>& triangles,
        const std::set<std::size_t> red_indices, const glhpp::Light& light) {
    auto triangles_count = triangles.size();
    vertices_.reserve(triangles_count * 3);
    for (std::size_t i = 0; i < triangles_count; ++i) {
      GLint color = red_indices.count(i);
      auto triangle = triangles[i];
      auto normal = triangle.normal();
      setOrientation(triangle, light.dir);

      vertices_.push_back({triangle.a_, normal, color});
      vertices_.push_back({triangle.b_, normal, color});
      vertices_.push_back({triangle.c_, normal, color});
    }
  }

  void setupRenderer(const glhpp::Renderer& renderer) const {
    renderer.setAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          offsetof(Vertex, point));
    renderer.setAttribute(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          offsetof(Vertex, normal));
    renderer.setAttribute(2, 1, GL_INT, GL_FALSE, sizeof(Vertex),
                          offsetof(Vertex, color_index));
  }
  const auto& getVertices() const noexcept { return vertices_; }

 private:
  void setOrientation(geometry::Triangle3D<float>& t,
                      const glm::vec3& light_dir) {
    auto normal = t.normal();
    glm::vec3 normal_glm{normal.x_, normal.y_, normal.z_};
    auto dot = glm::dot(normal_glm, light_dir);
    if (dot > 0.f) {
      std::swap(t.b_, t.c_);
    }
  }

 private:
  std::vector<Vertex> vertices_;
};
}
