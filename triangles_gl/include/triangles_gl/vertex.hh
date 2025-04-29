#pragma once

#include "GL/glew.h"
#include "geom/vector3d.hh"

namespace triangles_gl {

struct Vertex final {
  using Point = geometry::Vector3D<GLfloat>;

  Point point;
  Point normal;
  GLint color_index;  // to reduce memory usage
};

}
