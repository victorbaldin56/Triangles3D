#include <gtest/gtest.h>

#include "vector3d.hh"

using namespace geometry;

TEST(Vector3D, add) {
  Vector3D<float> v1{2, 1, 3};
  Vector3D<float> v2{1, 4.1, 5};
  ASSERT_TRUE((v1 + v2).isClose({3, 5.1, 8}));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
