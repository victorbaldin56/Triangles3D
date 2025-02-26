#ifndef TRIANGLES_3D_GEOMETRY_OCTREE_HH_
#define TRIANGLES_3D_GEOMETRY_OCTREE_HH_

#include <array>
#include <numeric>
#include <set>
#include <vector>
#include <utility>

#include "vector3d.hh"

namespace geometry {

// TODO: без рекурсии
// TODO: минимизировать raw pointers use
template <typename FigureT, typename Container = std::vector<FigureT>>
class Octree {
 public:
  using Iterator  = typename Container::iterator;
  using CIterator = typename Container::const_iterator;

 private:
  const Container cont_;


 public:
  // copy-constructs the underlying container
  explicit Octree(const Container& cont = Container{}) : cont_{cont} {
    constructTree();
  }

  // move-constructs the underlying container
  explicit Octree(Container&& cont) noexcept : cont_{cont} {
    constructTree();
  }

  std::set<std::size_t> getIntersectingFigures() const {
    std::set<std::size_t> res;
    root_->getIntersectingFigures(res);
    return res;
  }

  ~Octree() {
    delete root_;
  }

 private:
  struct Parallelepiped {
    NumType x_min_, x_max_;
    NumType y_min_, y_max_;
    NumType z_min_, z_max_;
  };

  struct Node {
    Parallelepiped coords_;

    Node* parent_;
    std::array<Node*, 8> children_{};

    void getIntersectingFigures(std::set<std::size_t>& res) {
      for (int i = 0; i < 8; ++i) {
        children_[i]->getIntersectingFigures(res);
      }
    }

    ~Node() {
      for (int i = 0; i < 8; ++i) {
        delete children_[i];
      }
    }
  };

  Node* root_ = nullptr;

  Parallelepiped getEdges() const {
    NumType min_init = std::numeric_limits<NumType>::max();
    NumType max_init = std::numeric_limits<NumType>::min();

    Parallelepiped res{min_init, max_init,
                       min_init, max_init,
                       min_init, max_init};

    for (auto& f : cont_) {
      NumType x_min = f.minX();
      NumType x_max = f.maxX();
      NumType y_min = f.minY();
      NumType y_max = f.maxY();
      NumType z_min = f.minZ();
      NumType z_max = f.maxZ();

      res.x_min_ = (x_min < res.x_min_ ? x_min : res.x_min_);
      res.x_max_ = (x_max > res.x_max_ ? x_max : res.x_max_);
      res.y_min_ = (y_min < res.y_min_ ? y_min : res.y_min_);
      res.y_max_ = (y_max > res.y_max_ ? y_max : res.y_max_);
      res.z_min_ = (z_min < res.z_min_ ? z_min : res.z_min_);
      res.z_max_ = (z_max > res.z_max_ ? x_min : res.z_max_);
    }

    return res;
  }

  void constructTree() {
    Parallelepiped edges = getEdges();
    root_ = new Node{edges};
  }
};

}

#endif // TRIANGLES_3D_GEOMETRY_OCTREE_HH_
