#include "triangles_gl/window.hh"

namespace triangles_gl {

void Window::handleEvents() {
  while (wnd_.isOpen()) {
    sf::Event event;
    while (wnd_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        wnd_.close();
      }
    }
  }
}

}  // namespace triangles_gl
