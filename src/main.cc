#include <stdexcept>
#include <vector>

//#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "geom/triangle3d.hh"
#include "geom/octree.hh"

int main() try {
  // for trace & debugging
  spdlog::set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));

  std::cin.exceptions(std::ios::failbit | std::ios::eofbit);
  auto count = std::size_t(0);
  std::cin >> count;

  auto triangles = std::vector<geometry::Triangle3D<double>>(count);
  if (count) {
    std::copy_n(std::istream_iterator<geometry::Triangle3D<double>>(std::cin),
                count, triangles.begin());
  }

  auto res = std::set<std::size_t>();

#if 1
  auto octree = geometry::Octree<double>(triangles.begin(), triangles.end());
  res = octree.getIntersections();
#else
  for (auto i = std::size_t(0); i < count; ++i) {
    for (auto j = i + 1; j < count; ++j) {
      auto&& ans = triangles[i].intersects(triangles[j]);
      if (ans) {
        res.insert(i + 1);
        res.insert(j + 1);
      }
    }
  }
#endif

  std::copy(res.begin(), res.end(),
            std::ostream_iterator<std::size_t>(std::cout, " "));
  std::cout << std::endl;
  return 0;
} catch (std::exception& ex) {
  std::cerr << ex.what() << std::endl;
  return EXIT_FAILURE;
}
