#pragma once

#include "SFML/Window.hpp"

namespace triangles_gl {

class Window final {
  sf::Window wnd_;

 public:
  Window(unsigned width, unsigned height, const std::string& title)
      : wnd_(sf::VideoMode(width, height), title) {}

  void handleEvents();
};

}
