#pragma once

#include <array>
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
    std::stack<pNode> node_stack_;
    bool valid_;

    Node(const Range3D<T>& coords,
         const InternalContainer& cont,
         const pNode& parent)
        : coords_(coords), triangles_(cont), parent_(parent) {}

    void divide() {
      node_stack_.push(this->shared_from_this());

      while (!node_stack_.empty()) {
        auto&& current_node = node_stack_.top();
        node_stack_.pop();

        if (current_node->triangles_.empty()) {
          continue;
        }

        Range3D<T> new_coords = coords_;
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

          children_[i] =
              std::make_shared<Node>(
                  new_coords, InternalContainer(), current_node);
        }

        for (auto it = triangles_.begin(); it != triangles_.end(); ++it) {
          (void)std::find_if(children_.begin(), children_.end(),
                             [&it, current_node, this](auto& ch) {
            auto&& tr = *it;

            if (ch->coords_.contains(tr.first.getRange())) {
              ch->triangles_.push_back(tr);
              ch->valid_ = true;
              node_stack_.push(ch);
              it = current_node->triangles_.erase(it);
              return true;
            }
            return false;
          });
        }
      }
    }

    std::set<std::size_t> getIntersections() {
      auto&& res = std::set<std::size_t>();

      while (!node_stack_.empty()) {
        auto&& current_node = node_stack_.top();
        node_stack_.pop();

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

        std::for_each(children_.cbegin(), children_.cend(),
                      [this](const auto& ch) {
          if (ch->valid_) {
            node_stack_.push(ch);
          }
        });
      }
      return res;
    }

    void getIntersectionsAmongChildren(
        std::set<std::size_t>& res,
        const std::pair<Triangle3D<T>, std::size_t>& triangle) {
      if (!valid_) {
        return;
      }

      node_stack_.push(this->shared_from_this());

      while (!node_stack_.empty()) {
        auto&& current_node = node_stack_.top();
        node_stack_.pop();

        std::for_each(children_.begin(), children_.end(),
                     [this, triangle, &res](const auto& ch) {
          if (ch->valid_) {
            std::for_each(triangles_.begin(), triangles_.end(),
                          [triangle, &res](const auto& other) {
              if (other.first.intersects(triangle.first)) {
                res.insert(other.second);
                res.insert(triangle.second);
              }
            });

            node_stack_.push(ch);
          }
        });
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
    InternalContainer triangles;

    std::size_t count = 1;
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
      triangles.push_back(std::make_pair(*it, count++));
    }

    root_ = std::make_shared<Node>(range, triangles, nullptr);
    root_->divide();
  }

  std::set<std::size_t> getIntersections() {
    return root_->getIntersections();
  }

 private:
  pNode root_;
};

}  // namespace geometry
