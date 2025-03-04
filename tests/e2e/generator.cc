#include <cmath>
#include <filesystem>
#include <iostream>
#include <random>
#include <set>
#include <vector>

#include "CGAL/Exact_predicates_exact_constructions_kernel.h"
#include "CGAL/intersections.h"

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
using Triangle_3 = Kernel::Triangle_3;
using Point_3 = Kernel::Point_3;

namespace fs = std::filesystem;

std::ostream& operator>>(std::ostream& os, const Triangle_3& t) {
  for (auto i = 0; i < 3; ++i) {
    auto&& v = t.vertex(i);
    os << v.x() << " " << v.y() << " " << v.z() << '\n';
  }
  return os;
}

class TestGenerator {
 public:
  TestGenerator()
      : rng_(rd_()),
        current_directory_(fs::absolute(__FILE__).parent_path()),
        input_directory_(fs::path(current_directory_).append(kInputDir)),
        ans_directory_(fs::path(current_directory_).append(kAnsDir)) {
    fs::create_directory(input_directory_);
    fs::create_directory(ans_directory_);
  }

  void run() {
    for (auto i = 0u; i < kNumTests; ++i) {
      generateTest(i);
    }
  }

 private:
  void generateTest(unsigned test_num) {
    auto&& cnt_dst =
        std::uniform_int_distribution<unsigned>(kMinTriangles, kMaxTriangles);
    auto&& triangles_count = cnt_dst(rng_);
    auto&& triangles = generateRandomTriangles(triangles_count);
    auto&& ans = getReferenceAnswer(triangles);

    writeTrianglesToFile(triangles, test_num);
    writeAnswerToFile(ans, test_num);

    std::cerr << "Test " << test_num << " generated\n";
  }

  std::vector<Triangle_3> generateRandomTriangles(unsigned count) {
    auto&& res = std::vector<Triangle_3>(count);
    std::transform(res.begin(), res.end(), res.begin(),
                   [this, count](const auto& e) {
      auto&& t =
          Triangle_3(Point_3(0, 0, 0), Point_3(0, 0, 0), Point_3(0, 0, 0));
      while (t.is_degenerate()) {
        t = generateRandomTriangle(count);
      }
      return t;
    });

    return res;
  }

  Triangle_3 generateRandomTriangle(unsigned triangles_count) {
    auto&& triangles_per_edge = static_cast<int>(std::cbrt(triangles_count));
    auto&& transfer_width =
        (kMaxCoord - kMinCoord) / ((triangles_per_edge + 1) * 2);
    auto&& trans_dist =
        std::uniform_int_distribution<int>(
            -triangles_per_edge, triangles_per_edge);

    auto&& tr_x = transfer_width * trans_dist(rng_);
    auto&& tr_y = transfer_width * trans_dist(rng_);
    auto&& tr_z = transfer_width * trans_dist(rng_);

    return Triangle_3(
        generateRandomPoint(tr_x, tr_y, tr_z),
        generateRandomPoint(tr_x, tr_y, tr_z),
        generateRandomPoint(tr_x, tr_y, tr_z));
  }

  Point_3 generateRandomPoint(double tr_x, double tr_y, double tr_z) {
    auto&& x_dst = std::uniform_real_distribution<double>(-tr_x, tr_x);
    auto&& y_dst = std::uniform_real_distribution<double>(-tr_y, tr_y);
    auto&& z_dst = std::uniform_real_distribution<double>(-tr_z, tr_z);
    auto&& x = x_dst(rng_) + tr_x + kCenterCoord;
    auto&& y = y_dst(rng_) + tr_y + kCenterCoord;
    auto&& z = z_dst(rng_) + tr_z + kCenterCoord;
    return Point_3(x, y, z);
  }

  std::set<unsigned> getReferenceAnswer(
      const std::vector<Triangle_3>& triangles) {
    auto&& res = std::set<unsigned>();
    auto&& size = triangles.size();
    for (auto i = 0u; i < size; ++i) {
      for (auto j = i + 1; j < size; ++j) {
        if (CGAL::do_intersect(triangles[i], triangles[j])) {
          res.insert(i + 1);
          res.insert(j + 1);
        }
      }
    }
    return res;
  }

  void writeTrianglesToFile(const std::vector<Triangle_3>& triangles,
                            unsigned test_num) {
    auto&& filepath =
        getOutputFileName(input_directory_, "test_", test_num, ".in");
    auto&& os = std::fstream();
    os.exceptions(std::ios::failbit);
    os.open(filepath, std::ios::trunc | std::ios::out);

    os << triangles.size() << '\n';
    std::copy(triangles.begin(), triangles.end(),
              std::ostream_iterator<Triangle_3>(os, "\n"));
  }

  void writeAnswerToFile(const std::set<unsigned>& ans, unsigned test_num) {
    auto&& filepath =
        getOutputFileName(ans_directory_, "ans_", test_num, ".out");
    auto&& os = std::fstream();
    os.exceptions(std::ios::failbit);
    os.open(filepath, std::ios::trunc | std::ios::out);
    std::copy(ans.begin(), ans.end(),
              std::ostream_iterator<unsigned>(os, " "));
  }

  static std::string getOutputFileName(const fs::path& dir,
                                       const char* prefix,
                                       unsigned test_num,
                                       const char* extension) {
    auto res = dir;
    auto filename = std::string(prefix) + std::to_string(test_num) + extension;
    res.append(filename);
    return res;
  }

 private:
  static constexpr auto kNumTests = 10u;
  static constexpr auto kInputDir = "input";
  static constexpr auto kAnsDir = "ans";
  static constexpr auto kMinTriangles = 1u;
  static constexpr auto kMaxTriangles = 300u;
  static constexpr auto kMinCoord = 0.0;
  static constexpr auto kMaxCoord = 1.0;
  static constexpr auto kCenterCoord = (kMinCoord + kMaxCoord) / 2;

  fs::path current_directory_;
  fs::path input_directory_;
  fs::path ans_directory_;

  std::random_device rd_;
  std::mt19937_64 rng_;
};

int main() {
  auto&& gen = TestGenerator();
  gen.run();
  return 0;
}
