#pragma once

#include <array>
#include <bitset>
#include <iterator>
#include <list>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <type_traits>

#include "boost/asio/post.hpp"
#include "boost/asio/thread_pool.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/thread.hpp"

#include "spdlog/spdlog.h"

#include "range3d.hh"
#include "triangle3d.hh"

namespace geometry {

namespace detail {

inline boost::asio::thread_pool createThreadPool() {
  return boost::asio::thread_pool(boost::thread::hardware_concurrency());
}

}  // namespace detail

template <typename T, typename = std::enable_if<std::is_floating_point_v<T>>>
class Octree final {
  struct Node;
  using pNode = std::shared_ptr<Node>;
  using pConstNode = std::shared_ptr<const Node>;

  // because we need to erase elements from middle
  using InternalContainer = std::list<std::pair<Triangle3D<T>, std::size_t>>;

 private:
  /**
   * Octree node representing octant. Managed by std::shared_ptr.
   */
  struct Node final : public std::enable_shared_from_this<Node> {
    Range3D<T> coords_;
    InternalContainer triangles_;
    std::array<pNode, 8> children_;
    std::bitset<8> valid_children_;

    Node(const Range3D<T>& coords) noexcept : coords_(coords) {}

    void partition() {
      auto thread_pool = detail::createThreadPool();
      auto node_stack = std::stack<pNode>();
      auto queue_mutex = boost::mutex();
      auto num_threads = boost::thread::hardware_concurrency();

      node_stack.push(this->shared_from_this());

      auto task = [&] {
        while (!node_stack.empty()) {
          SPDLOG_TRACE("Stack size = {}", queue.size());
          auto current_node = pNode();
          {
            auto lock = boost::unique_lock<boost::mutex>(queue_mutex);
            current_node = node_stack.top();
            node_stack.pop();
          }

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

            current_node->children_[i] = std::make_shared<Node>(new_coords);
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

          {
            auto lock = boost::lock_guard<boost::mutex>(queue_mutex);
            for (auto ch = 0; ch < 8; ++ch) {
              if (!current_node->children_[ch]->triangles_.empty()) {
                current_node->valid_children_[ch] = true;
                node_stack.push(current_node->children_[ch]);
              } else {
                current_node->valid_children_[ch] = false;
              }
            }
          }
        }
      };

      boost::asio::post(thread_pool, task);
      thread_pool.join();
    }

    std::set<std::size_t> getIntersections() const {
      auto thread_pool = detail::createThreadPool();
      auto node_stack = std::stack<pConstNode>();
      node_stack.push(this->shared_from_this());
      auto stack_mutex = boost::mutex();

      auto res = std::set<std::size_t>();
      auto res_mutex = boost::mutex();

      auto task = [&] {
        while (!node_stack.empty()) {
          auto current_node = pConstNode();
          {
            auto lock = boost::lock_guard<boost::mutex>(stack_mutex);
            current_node = node_stack.top();
            node_stack.pop();
          }

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
                auto lock = boost::lock_guard<boost::mutex>(res_mutex);
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
              node_stack.push(current_node->children_[i]);
            }
          }
        }
      };

      boost::asio::post(thread_pool, task);
      thread_pool.join();

      return res;
    }

    void getIntersectionsAmongChildren(
        std::set<std::size_t>& res,
        const std::pair<Triangle3D<T>, std::size_t>& triangle) const {
      if (valid_children_ == 0) {
        return;
      }

      auto node_stack = std::stack<pConstNode>();
      node_stack.push(this->shared_from_this());

      while (!node_stack.empty()) {
        auto current_node = node_stack.top();
        node_stack.pop();

        for (auto ch = 0; ch < 8; ++ch) {
          if (current_node->valid_children_[ch]) {
            auto&& current_triangles = current_node->children_[ch]->triangles_;
            auto triangles_begin = current_triangles.begin();
            auto triangles_end = current_triangles.end();

            std::for_each(triangles_begin, triangles_end,
                          [&](const auto& other) {
              if (other.first.intersects(triangle.first)) {
                res.insert(other.second);
                res.insert(triangle.second);

                SPDLOG_TRACE("Triangles {} and {} intersect",
                              triangle.second, other.second);
              }
            });

            node_stack.push(current_node->children_[ch]);
          }
        }
      }
    }
  };

 public:
  template <
      typename It,
      typename
          = std::enable_if<std::is_base_of_v<std::input_iterator_tag, It>>>
  Octree(It begin, It end, std::size_t min_size = kMinSize) {
    if (begin == end) {
      return;
    }

    auto&& range = begin->getRange();
    root_ = std::make_shared<Node>(Range3D<T>{});

    auto count = std::size_t{0};
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
      if (range.max_z_ < cur.max_z_) {
        range.max_z_ = cur.max_z_;
      }
      root_->triangles_.emplace_front(*it, count++);
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
  pNode root_;

 private:
  /** min number of triangles inside node */
  static constexpr auto kMinSize = std::size_t{0x1};
};

}  // namespace geometry
