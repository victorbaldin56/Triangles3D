#include <vector>

#include "triangle3d.hh"

int main() {
  std::size_t count = 0;
  std::cin >> count;

  std::vector<geometry::Triangle3D<float>> triangles(count);
  for (auto& t : triangles) {
    std::cin >> t;
  }

  return 0;
}
