#include <stdexcept>
#include <vector>

// #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "driver/cmd_parser.hh"
#include "geom/octree.hh"
#include "geom/triangle3d.hh"
#include "glhpp/gl.hh"
#include "triangles_gl/window.hh"

int main(int argc, char** argv) try {
  cmd::CmdParser parser(argc, argv);
  auto cfg = parser.run();

  // for trace and debugging
  spdlog::set_level(
      static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));

  std::size_t count;
  std::cin >> count;
  if (!std::cin) {
    throw std::runtime_error("Unexpected EOF");
  }

  std::vector<geometry::Triangle3D<double>> triangles(
      (std::istream_iterator<geometry::Triangle3D<double>>(std::cin)),
      (std::istream_iterator<geometry::Triangle3D<double>>()));
  if (triangles.size() != count) {
    throw std::runtime_error(
        "Number of inputted triangles and initially inputted count mismatch");
  }

  geometry::Octree<double> octree(triangles.cbegin(), triangles.cend());
  auto res = octree.getIntersections();
  if (cfg.draw) {
    glhpp::init();
    triangles_gl::Window wnd(700, 700, "Triangles3D");
    wnd.pollInLoop();
  } else {
    std::copy(res.begin(), res.end(),
              std::ostream_iterator<std::size_t>(std::cout, "\n"));
  }
  return 0;
} catch (std::exception& ex) {
  std::cerr << ex.what() << std::endl;
  return EXIT_FAILURE;
}
