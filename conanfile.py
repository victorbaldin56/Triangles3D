from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain
import pip

class Triangles3DRecipe(ConanFile):
  name = "triangles3d"
  version = "1.0"
  settings = "os", "compiler", "build_type", "arch"
  generators = "CMakeDeps"

  def requirements(self):
    self.requires("spdlog/1.15.1")
    self.test_requires("gtest/1.15.0")
    self.test_requires("cgal/6.0.1")

  def generate(self):
    # Customize CMakeToolchain in the generate() method
    tc = CMakeToolchain(self)
    tc.generate()

  def build(self):
    cmake = CMake(self)
    cmake.configure()
    cmake.build()
    cmake.test()
