#pragma once

#include "SFML/Window.hpp"
#include "camera.hh"
#include "glhpp/renderer.hh"
#include "spdlog/spdlog.h"

namespace triangles_gl {

struct Mouse final {
  float rotate_speed = 0.f;
  bool is_active = false;
  bool is_centered = false;
  sf::Vector2i pos = {0, 0};

  static constexpr auto kMaxRotateSpeed = 1e-1;
  static constexpr auto kDeltaSpeedRotate = 5e-1;
};

struct Keyboard final {
  float translate_speed = 0.1f;
};

class Window final {
  sf::Window wnd_;
  Mouse mouse_;
  Keyboard keyboard_;

 public:
  Window(unsigned width, unsigned height, const std::string& title)
      : wnd_(sf::VideoMode(width, height), title, sf::Style::Default,
             sf::ContextSettings(24, 8, 0, 3, 3)) {
    wnd_.setVerticalSyncEnabled(true);
  }

  auto getSize() const noexcept { return wnd_.getSize(); }

  auto pollInLoop(const glhpp::Renderer& renderer, Camera& camera) {
    while (wnd_.isOpen()) {
      sf::Event event;
      while (wnd_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          return;
        }
        handleEvent(event, renderer, camera);
      }
      auto size = getSize();
      renderer.render(camera.getPerspective(size.x, size.y), camera.getLookAt(),
                      GL_TRIANGLES);
      wnd_.display();
    }
  }

 private:
  void handleEvent(const sf::Event& evt, const glhpp::Renderer& renderer,
                   Camera& camera) {
    auto size = getSize();
    switch (evt.type) {
      case sf::Event::Resized:
        renderer.resize(size.x, size.y);
        break;
      case sf::Event::KeyPressed:
        handleKey(evt.key.code, renderer, camera);
        break;
      case sf::Event::MouseMoved:
        handleMouseMove(camera);
        break;
      case sf::Event::MouseWheelScrolled:
        handleMouseScroll(evt.mouseWheelScroll.delta, camera);
      default:
        break;
    }
  }

  void handleKey(sf::Keyboard::Key code, const glhpp::Renderer& renderer,
                 Camera& camera) {
    auto right = camera.getSideways();
    auto direction = camera.getDirection();
    switch (code) {
      case sf::Keyboard::A:
        camera.translate(-right * keyboard_.translate_speed);
        break;
      case sf::Keyboard::D:
        camera.translate(right * keyboard_.translate_speed);
        break;
      case sf::Keyboard::W:
        camera.translate(direction * keyboard_.translate_speed);
        break;
      case sf::Keyboard::S:
        camera.translate(-direction * keyboard_.translate_speed);
        break;
      default:
        break;
    }
  }

  void handleMouseMove(Camera& camera) {
    auto mouse_pos = sf::Mouse::getPosition(wnd_);
    auto delta = mouse_pos - mouse_.pos;

    auto yaw = mouse_.rotate_speed * delta.x;
    auto pitch = mouse_.rotate_speed * delta.y;
    glm::vec3 right =
        glm::normalize(glm::cross(camera.getDirection(), camera.getUp()));
    auto q_yaw = glm::angleAxis(glm::radians(-yaw), glm::vec3(0.f, 1.f, 0.f));
    auto q_pitch = glm::angleAxis(glm::radians(-pitch), right);
    auto rotation = glm::normalize(q_pitch * q_yaw);
    camera.rotate(rotation);
    mouse_.rotate_speed = std::min(
        Mouse::kMaxRotateSpeed, mouse_.rotate_speed + Mouse::kDeltaSpeedRotate);
    mouse_.pos = mouse_pos;
  }

  void handleMouseScroll(float delta, Camera& camera) { camera.scale(delta); }
};

}
