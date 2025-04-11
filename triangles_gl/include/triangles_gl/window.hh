#pragma once

#include "SFML/Window.hpp"
#include "camera.hh"
#include "renderer.hh"

namespace triangles_gl {

class Window final {
  sf::Window wnd_;

 public:
  Window(unsigned width, unsigned height, const std::string& title)
      : wnd_(sf::VideoMode(width, height), title) {}

  auto getSize() const noexcept { return wnd_.getSize(); }

  auto pollInLoop(const Renderer& renderer, Camera& camera) {
    while (wnd_.isOpen()) {
      sf::Event event;
      while (wnd_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          return;
        }
        handleEvent(event, renderer, camera);
      }

      auto [width, height] = getSize();
      renderer.render(camera.getPerspective(width, height), camera.getLookAt());
      wnd_.display();
    }
  }

 private:
  void handleEvent(const sf::Event& evt, const Renderer& renderer,
                   Camera& camera) {
    auto size = getSize();
    switch (evt.type) {
      case sf::Event::Resized:
        renderer.resize(size.x, size.y);
      default:
        break;
    }
  }
};

}
