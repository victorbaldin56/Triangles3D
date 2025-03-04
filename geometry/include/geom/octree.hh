#pragma once

#include <array>
#include <bitset>
#include <iterator>
#include <list>
#include <memory>
#include <set>
#include <stack>
#include <type_traits>

#include "range3d.hh"
#include "triangle3d.hh"

namespace geometry {

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
class Octree final {
  struct Node;
  using pNode = std::shared_ptr<Node>;
  using InternalContainer = std::list<std::pair<Triangle3D<T>, std::size_t>>;

 private:
  struct Node final : public std::enable_shared_from_this<Node> {
    Range3D<T> coords_;
    InternalContainer triangles_;
    pNode parent_;
    std::array<pNode, 8> children_;
    std::bitset<8> valid_children_;

    Node(const Range3D<T>& coords,
         const pNode& parent)
        : coords_(coords), parent_(parent) {}

    void divide() {
      auto&& node_stack = std::stack<pNode>();
      node_stack.push(this->shared_from_this());

      while (!node_stack.empty()) {
        auto current_node = node_stack.top();
        node_stack.pop();

        if (current_node->triangles_.empty()) {
          continue;
        }

        auto new_coords = coords_;
        for (auto i = 0; i < 8; ++i) {
          if (i & 0x1) {
            new_coords.min_x_ += coords_.dimX() / 2;
          } else {
            new_coords.max_x_ -= coords_.dimX() / 2;
          }

          if (i & 0x3) {
            new_coords.min_y_ += coords_.dimY() / 2;
          } else {
            new_coords.max_y_ -= coords_.dimY() / 2;
          }

          if (i & 0x7) {
            new_coords.min_z_ += coords_.dimZ() / 2;
          } else {
            new_coords.max_z_ -= coords_.dimZ() / 2;
          }

          children_[i] = std::make_shared<Node>(new_coords, current_node);
        }

        for (auto it = triangles_.begin(); it != triangles_.end(); ++it) {
          for (auto i = 0; i < 8; ++i) {
            auto tr = *it;
            auto&& ch = children_[i];

            if (ch->coords_.contains(tr.first.getRange())) {
              ch->triangles_.push_back(tr);
              valid_children_[i] = true;
              node_stack.push(ch);
              it = current_node->triangles_.erase(it);
              break;
            }
          };
        }
      }
    }

    std::set<std::size_t> getIntersections() {
      auto&& node_stack = std::stack<pNode>();
      auto&& res = std::set<std::size_t>();

      while (!node_stack.empty()) {
        auto current_node = node_stack.top();
        node_stack.pop();

        for (auto it = triangles_.cbegin(); it != triangles_.cend(); ++it) {
          auto&& tr1 = *it;
          for (auto jt = it; jt != triangles_.cend(); ++jt) {
            auto&& tr2 = *jt;
            if (tr1.second == tr2.second) {
              continue;
            }
            if (tr1.first.intersects(tr2.first)) {
              res.insert(tr1.second);
              res.insert(tr2.second);
            }
          }

          getIntersectionsAmongChildren(res, tr1);
        }

        for (auto i = 0; i < 8; ++i) {
          if (valid_children_[i]) {
            node_stack.push(children_[i]);
          }
        };
      }
      return res;
    }

    void getIntersectionsAmongChildren(
        std::set<std::size_t>& res,
        const std::pair<Triangle3D<T>, std::size_t>& triangle) {
      auto&& node_stack = std::stack<pNode>();
      node_stack.push(this->shared_from_this());

      while (!node_stack.empty()) {
        auto&& current_node = node_stack.top();
        node_stack.pop();

        for (auto ch = 0; ch < 8; ++ch) {
          if (valid_children_[ch]) {
            std::for_each(triangles_.begin(), triangles_.end(),
                          [triangle, &res, &node_stack](const auto& other) {
              if (other.first.intersects(triangle.first)) {
                res.insert(other.second);
                res.insert(triangle.second);
              }
            });

            node_stack.push(children_[ch]);
          }
        };
      }
    }
  };

 public:
  template <
      typename It,
      typename
          = std::enable_if<std::is_base_of_v<std::input_iterator_tag, It>>>
  Octree(It begin, It end) {
    auto&& range = begin->getRange();
    root_ = std::make_shared<Node>(Range3D<T>{}, nullptr);

    std::size_t count = 0;
    for (auto it = begin; it != end; ++it) {
      auto&& cur = it->getRange();

      if (cur.min_x_ < range.min_x_) {
        range.min_x_ = cur.min_x_;
      }
      if (range.max_x_ < cur.max_x_) {
        range.max_x_ = cur.max_x_;
      }

      if (cur.min_y_ < range.min_y_) {
        range.min_y_ = cur.min_y_;
      }
      if (range.max_y_ < cur.max_y_) {
        range.max_y_ = cur.max_y_;
      }

      if (cur.min_z_ < range.min_z_) {
        range.min_z_ = cur.min_z_;
      }
      if (range.max_x_ < cur.max_x_) {
        range.max_z_ = cur.max_z_;
      }
      root_->triangles_.push_back(std::make_pair(*it, ++count));
    }

    root_->coords_ = range;
    root_->divide();
  }

  std::set<std::size_t> getIntersections() {
    return root_->getIntersections();
  }

 private:
  pNode root_;
};

}  // namespace geometry
