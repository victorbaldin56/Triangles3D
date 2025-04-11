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

    glhpp::init();
    auto shaders_path =
        std::filesystem::absolute(__FILE__).parent_path().append("shaders");
    std::vector<glhpp::Shader> triangles_shaders;
    triangles_shaders.emplace_back(shaders_path.append("triangles.vert"),
                                   GL_VERTEX_SHADER);
    triangles_shaders.emplace_back(shaders_path.append("triangles.frag"),
                                   GL_FRAGMENT_SHADER);

    std::vector<glhpp::Shader> shadow_shaders;
    shadow_shaders.emplace_back(shaders_path.append("shadow_map.vert"),
                                GL_VERTEX_SHADER);
    shadow_shaders.emplace_back(shaders_path.append("shadow_map.frag"),
                                GL_FRAGMENT_SHADER);

    triangles_gl::Scene scene(triangles, indices);
    triangles_gl::Renderer renderer(triangles_shaders, shadow_shaders,
                                    scene.getVertices(), {}, kWindowWidth,
                                    kWindowHeight);

    triangles_gl::Camera camera;

    triangles_gl::Window wnd(kWindowWidth, kWindowHeight, "Triangles3D");
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
