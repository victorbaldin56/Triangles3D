#include <stdexcept>
#include <vector>

// #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "driver/cmd_parser.hh"
#include "geom/octree.hh"
#include "geom/triangle3d.hh"
#include "glhpp/gl.hh"
#include "glhpp/shader.hh"
#include "triangles_gl/scene.hh"
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

  std::vector<geometry::Triangle3D<float>> triangles(
      (std::istream_iterator<geometry::Triangle3D<float>>(std::cin)),
      (std::istream_iterator<geometry::Triangle3D<float>>()));
  if (triangles.size() != count) {
    throw std::runtime_error(
        "Number of inputted triangles and initially inputted count mismatch");
  }

  geometry::Octree<float> octree(triangles.cbegin(), triangles.cend());
  auto indices = octree.getIntersections();
  if (cfg.draw) {
    constexpr auto kWindowWidth = 700u;
    constexpr auto kWindowHeight = 700u;
    triangles_gl::Window wnd(kWindowWidth, kWindowHeight, "Triangles3D");

    glhpp::init();
    auto shaders_path =
        std::filesystem::absolute(__FILE__).parent_path().append("shaders");
    std::vector<glhpp::Shader> triangles_shaders;
    triangles_shaders.emplace_back(shaders_path / "triangles.vert",
                                   GL_VERTEX_SHADER);
    triangles_shaders.emplace_back(shaders_path / "triangles.frag",
                                   GL_FRAGMENT_SHADER);

    std::vector<glhpp::Shader> shadow_shaders;
    shadow_shaders.emplace_back(shaders_path / "shadow_map.vert",
                                GL_VERTEX_SHADER);
    shadow_shaders.emplace_back(shaders_path / "shadow_map.frag",
                                GL_FRAGMENT_SHADER);

    triangles_gl::Scene scene(triangles, indices);
    glhpp::Renderer renderer(
        triangles_shaders, shadow_shaders, scene.getVertices(),
        {.width = 2048,
         .height = 2048,
         .dir = {-1, -1, -1},
         .pos = {2, 2, 2},
         .up = {0, 1, 0},
         .projection_matrix = glm::ortho<float>(-1.4, 1.4, -1.4, 1.4, 0.1, 5)},
        kWindowWidth, kWindowHeight, GL_TRIANGLES);

    triangles_gl::Camera camera({0.f, 0.f, -7.f}, {0.f, 0.f, 1.f},
                                {0.f, 1.f, 0.0}, glm::radians(45.f), 0.1f,
                                70.f);

    wnd.pollInLoop(renderer, camera);
  } else {
    std::copy(indices.begin(), indices.end(),
              std::ostream_iterator<std::size_t>(std::cout, "\n"));
  }
  return 0;
} catch (std::exception& ex) {
  std::cerr << ex.what() << std::endl;
  return EXIT_FAILURE;
}
