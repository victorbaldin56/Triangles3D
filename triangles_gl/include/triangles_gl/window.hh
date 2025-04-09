#pragma once

#include "SFML/Window.hpp"

namespace triangles_gl {

class Window final {
  sf::Window wnd_;

 public:
  Window(unsigned width, unsigned height)
      : wnd_(sf::VideoMode(width, height), "Triangles3D", sf::Style::Close) {}

  void handleEvents();
};

}
