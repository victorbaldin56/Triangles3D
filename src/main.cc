#include <stdexcept>
#include <vector>

//#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "geom/triangle3d.hh"
#include "geom/octree.hh"

int main() try {
  // for trace & debugging
  spdlog::set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));

  std::cin.exceptions(std::ios::failbit | std::ios::eofbit);
  auto count = std::size_t{};
  std::cin >> count;

  auto triangles = std::vector<geometry::Triangle3D<double>>();
  triangles.reserve(count);
  while (triangles.size() < count) {
    auto cur = geometry::Triangle3D<double>{};
    std::cin >> cur;
    triangles.push_back(cur);
  }

  auto octree = geometry::Octree<double>(triangles.begin(), triangles.end());
  auto res = octree.getIntersections();
  std::copy(res.begin(), res.end(),
            std::ostream_iterator<std::size_t>(std::cout, "\n"));
  return 0;
} catch (std::exception& ex) {
  std::cerr << ex.what() << std::endl;
  return EXIT_FAILURE;
}
