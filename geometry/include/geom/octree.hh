#pragma once

#include <array>
#include <bitset>
#include <iterator>
#include <list>
#include <memory>
#include <set>
#include <stack>
#include <type_traits>

#include "spdlog/spdlog.h"

#include "range3d.hh"
#include "triangle3d.hh"

namespace geometry {

template <typename T>
class Octree final {
  // because we need to erase elements from middle
  using InternalContainer = std::list<std::pair<Triangle3D<T>, std::size_t>>;

 private:
  /**
   * Octree node representing octant. Managed by std::shared_ptr.
   */
  struct Node final {
    Range3D<T> coords_;
    InternalContainer triangles_;
    std::array<std::unique_ptr<Node>, 8> children_;
    std::bitset<8> valid_children_;

    Node(const Range3D<T>& coords) noexcept : coords_(coords) {}

    void partition() {
      std::stack<Node*> node_stack;
      node_stack.push(this);

      while (!node_stack.empty()) {
        SPDLOG_TRACE("Stack size = {}", node_stack.size());

        auto current_node = node_stack.top();
        node_stack.pop();

        SPDLOG_TRACE("current_node.get() = {}",
                     static_cast<void*>(current_node.get()));

        auto begin_size = current_node->triangles_.size();
        if (begin_size <= kMinSize) {
          continue;
        }

        auto&& coords = current_node->coords_;
        auto mid_x = (coords.min_x_ + coords.max_x_) / 2;
        auto mid_y = (coords.min_y_ + coords.max_y_) / 2;
        auto mid_z = (coords.min_z_ + coords.max_z_) / 2;

        for (auto i = 0; i < 8; ++i) {
          auto new_coords = coords;

          if (i & 0x1) {
            new_coords.max_x_ = mid_x;
          } else {
            new_coords.min_x_ = mid_x;
          }

          if (i & 0x2) {
            new_coords.max_y_ = mid_y;
          } else {
            new_coords.min_y_ = mid_y;
          }

          if (i & 0x4) {
            new_coords.max_z_ = mid_z;
          } else {
            new_coords.min_z_ = mid_z;
          }

          current_node->children_[i] = std::make_unique<Node>(new_coords);
        }

        auto&& current_triangles = current_node->triangles_;
        auto triangles_begin = current_triangles.begin();
        auto triangles_end = current_triangles.end();

        current_triangles.remove_if([&current_node](auto& tr) {
          auto moved = false;
          auto range = tr.first.getRange();

          for (auto i = 0; i < 8; ++i) {
            auto&& ch = current_node->children_[i];
            if (ch->coords_.contains(range)) {
              SPDLOG_TRACE("Moving triangle {} to child {}", tr.second, i);

              current_node->valid_children_[i] = true;
              ch->triangles_.push_front(tr);
              moved = true;
            }
          }
          return moved;
        });

        for (auto ch = 0; ch < 8; ++ch) {
          if (!current_node->children_[ch]->triangles_.empty()) {
            current_node->valid_children_[ch] = true;
            node_stack.push(current_node->children_[ch].get());
          } else {
            current_node->valid_children_[ch] = false;
          }
        }
      }
    }

    std::set<std::size_t> getIntersections() const {
      std::stack<const Node*> node_stack;
      node_stack.push(this);

      std::set<std::size_t> res;

      while (!node_stack.empty()) {
        auto current_node = node_stack.top();
        node_stack.pop();

        auto triangles_begin = current_node->triangles_.begin();
        auto triangles_end = current_node->triangles_.end();

        for (auto it = triangles_begin; it != triangles_end; ++it) {
          auto&& tr1 = *it;
          for (auto jt = it; jt != triangles_end; ++jt) {
            auto&& tr2 = *jt;
            if (tr1.second == tr2.second) {
              continue;
            }
            if (tr1.first.intersects(tr2.first)) {
              res.insert(tr1.second);
              res.insert(tr2.second);

              SPDLOG_TRACE("Triangles {} and {} intersect",
                           tr1.second, tr2.second);
            }
          }

          current_node->getIntersectionsAmongChildren(res, tr1);
        }

        for (auto i = 0; i < 8; ++i) {
          if (current_node->valid_children_[i]) {
            node_stack.push(current_node->children_[i].get());
          }
        }
      }
      return res;
    }

    void getIntersectionsAmongChildren(
        std::set<std::size_t>& res,
        const std::pair<Triangle3D<T>, std::size_t>& triangle) const {
      if (valid_children_ == 0) {
        return;
      }

      std::stack<const Node*> node_stack;
      node_stack.push(this);

      while (!node_stack.empty()) {
        auto current_node = node_stack.top();
        node_stack.pop();

        for (auto ch = 0; ch < 8; ++ch) {
          if (current_node->valid_children_[ch]) {
            auto&& current_triangles = current_node->children_[ch]->triangles_;
            auto triangles_begin = current_triangles.begin();
            auto triangles_end = current_triangles.end();

            std::for_each(triangles_begin, triangles_end,
                          [triangle, &res](const auto& other) {
              if (other.first.intersects(triangle.first)) {
                res.insert(other.second);
                res.insert(triangle.second);

                SPDLOG_TRACE("Triangles {} and {} intersect",
                              triangle.second, other.second);
              }
            });

            node_stack.push(current_node->children_[ch].get());
          }
        }
      }
    }
  };

 public:
  template <
      typename It,
      typename =
          std::enable_if_t<
              std::is_base_of_v<std::input_iterator_tag,
              typename std::iterator_traits<It>::iterator_category>>>
  Octree(It begin, std::size_t n, std::size_t min_size = kMinSize) {
    if (!n) {
      return;
    }

    auto range = begin->getRange();
    root_ = std::make_unique<Node>(Range3D<T>{});

    for (std::size_t count = 0; count != n; ++begin, ++count) {
      auto&& tr = *begin;
      auto cur = tr.getRange();

      range.min_x_ = std::min(range.min_x_, cur.min_x_);
      range.max_x_ = std::max(range.max_x_, cur.max_x_);
      range.min_y_ = std::min(range.min_y_, cur.min_y_);
      range.max_y_ = std::max(range.max_y_, cur.max_y_);
      range.min_z_ = std::min(range.min_z_, cur.min_z_);
      range.max_z_ = std::max(range.max_z_, cur.max_z_);

      root_->triangles_.emplace_front(tr, count);
    }

    root_->coords_ = range;
    root_->partition();
  }

  std::set<std::size_t> getIntersections() const {
    if (root_) {
      return root_->getIntersections();
    }
    return std::set<std::size_t>();
  }

 private:
  std::unique_ptr<Node> root_;

 private:
  /** min number of triangles inside node */
  static constexpr std::size_t kMinSize = 0x100;
};

}  // namespace geometry
