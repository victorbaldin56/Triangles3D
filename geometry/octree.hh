#ifndef TRIANGLES_3D_GEOMETRY_OCTREE_HH_
#define TRIANGLES_3D_GEOMETRY_OCTREE_HH_

#include <vector>

namespace geometry {

template <typename Figure>
class Octree {
  std::vector<Figure> figures_;

 public:
  Octree() {}
};

}

#endif // TRIANGLES_3D_GEOMETRY_OCTREE_HH_
