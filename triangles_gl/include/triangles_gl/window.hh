#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window.hpp"
#include "camera.hh"
#include "glhpp/renderer.hh"
#include "imgui-SFML.h"
#include "imgui.h"
#include "spdlog/spdlog.h"

namespace triangles_gl {

struct Mouse final {
  float rotate_speed = 0.f;
  bool is_active = false;
  bool is_centered = false;
  sf::Vector2i pos = {0, 0};
  float zoom_speed = 0.05f;

  static constexpr auto kMaxRotateSpeed = 0.2f;
  static constexpr auto kDeltaRotateSpeed = 1.f;
};

struct Keyboard final {
  float translate_speed = 0.1f;
};

class Window final {
 public:
  Window(unsigned width, unsigned height, const std::string& title)
      : wnd_(sf::VideoMode(width, height), title, sf::Style::Default,
             sf::ContextSettings(24, 8, 0, 3, 3)) {
    wnd_.setVerticalSyncEnabled(true);
    if (!ImGui::SFML::Init(wnd_)) {
      throw std::runtime_error{"Failed to initialize ImGui"};
    }
  }

  auto getSize() const noexcept { return wnd_.getSize(); }

  auto pollInLoop(const glhpp::Renderer& renderer, Camera& camera) {
    sf::Clock delta_clock;
    while (wnd_.isOpen()) {
      sf::Event event;
      while (wnd_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          return;
        }
        ImGui::SFML::ProcessEvent(wnd_, event);
        handleEvent(event, renderer, camera);
      }

      ImGui::SFML::Update(wnd_, delta_clock.restart());

      ImGui::Begin("Performance");
      ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
      ImGui::End();

      wnd_.clear();
      ImGui::SFML::Render(wnd_);
      auto size = getSize();
      renderer.render(camera.getPerspective(size.x, size.y),
                      camera.getLookAt());
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
      case sf::Event::MouseButtonPressed:
        mouse_control_active_ = !mouse_control_active_;
        break;
      case sf::Event::MouseWheelScrolled:
        handleMouseScroll(evt.mouseWheelScroll.delta, camera);
      default:
        break;
    }
  }

  void handleKey(sf::Keyboard::Key code, const glhpp::Renderer& renderer,
                 Camera& camera) {
    auto right = camera.getRight();
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
    if (wnd_.hasFocus() && mouse_control_active_) {
      auto mouse_pos = sf::Mouse::getPosition(wnd_);
      auto delta = mouse_pos - mouse_.pos;

      auto yaw = mouse_.rotate_speed * delta.x;
      auto pitch = mouse_.rotate_speed * delta.y;
      glm::vec3 right = camera.getRight();
      auto q_yaw = glm::angleAxis(glm::radians(-yaw), glm::vec3(0.f, 1.f, 0.f));
      auto q_pitch = glm::angleAxis(glm::radians(-pitch), right);
      auto rotation = glm::normalize(q_pitch * q_yaw);
      camera.rotate(rotation);
      mouse_.rotate_speed =
          std::min(Mouse::kMaxRotateSpeed,
                   mouse_.rotate_speed + Mouse::kDeltaRotateSpeed);
      mouse_.pos = mouse_pos;
    }
  }

  void handleMouseScroll(float delta, Camera& camera) noexcept {
    camera.scale(delta * mouse_.zoom_speed);
  }

 private:
  sf::RenderWindow wnd_;
  Mouse mouse_;
  Keyboard keyboard_;
  bool mouse_control_active_ = true;
};
}
