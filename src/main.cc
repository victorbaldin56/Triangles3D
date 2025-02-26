#include <vector>

#include "geom/triangle3d.hh"
#include "geom/octree.hh"

int main() {
  std::size_t count = 0;
  std::cin >> count;

  std::vector<geometry::Triangle3D<double>> triangles(count);
  for (auto& t : triangles) {
    std::cin >> t;
  }

  // we dont need to copy triangles vector
  geometry::Octree<geometry::Triangle3D<double>> octree{std::move(triangles)};
  return 0;
}
