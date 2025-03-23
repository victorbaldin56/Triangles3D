#include <stdexcept>
#include <vector>

// #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "geom/triangle3d.hh"
#include "geom/octree.hh"

int main() try {
  // for trace & debugging
  spdlog::set_level(
      static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
  auto count = std::size_t{};
  std::cin >> count;

  auto octree =
      geometry::Octree<double>(
          std::istream_iterator<geometry::Triangle3D<double>>(std::cin),
          count);
  auto res = octree.getIntersections();
  std::copy(res.begin(), res.end(),
            std::ostream_iterator<std::size_t>(std::cout, "\n"));
  return 0;
} catch (std::exception& ex) {
  std::cerr << ex.what() << std::endl;
  return EXIT_FAILURE;
}
