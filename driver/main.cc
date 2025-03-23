#include <stdexcept>
#include <vector>

// #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "geom/triangle3d.hh"
#include "geom/octree.hh"

int main() try {
  // for trace and debugging
  spdlog::set_level(
      static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));

  std::size_t count;
  std::cin >> count;
  if (!std::cin) {
    throw std::runtime_error("Unexpected EOF");
  }

  geometry::Octree<double> octree(
      (std::istream_iterator<geometry::Triangle3D<double>>(std::cin)),
      (std::istream_iterator<geometry::Triangle3D<double>>()));
  if (octree.size() != count) {
    throw std::runtime_error(
        "Number of inputted triangles and initially inputted count mismatch");
  }
  auto res = octree.getIntersections();
  std::copy(res.begin(), res.end(),
            std::ostream_iterator<std::size_t>(std::cout, "\n"));
  return 0;
} catch (std::exception& ex) {
  std::cerr << ex.what() << std::endl;
  return EXIT_FAILURE;
}
