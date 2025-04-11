#pragma once

#include "SFML/Window.hpp"
#include "camera.hh"

namespace triangles_gl {

class Window final {
  sf::Window wnd_;

 public:
  Window(unsigned width, unsigned height, const std::string& title)
      : wnd_(sf::VideoMode(width, height), title) {}

  void handleEvents() {
    while (wnd_.isOpen()) {
      sf::Event event;
      while (wnd_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          wnd_.close();
        }
      }
    }
  }
};

}
