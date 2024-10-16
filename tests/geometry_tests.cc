#include <gtest/gtest.h>

#include "vector3d.hh"
#include "plane.hh"

using namespace geometry;

TEST(Vector3D, add) {
  Vector3D<float> v1{2, 1, 3};
  Vector3D<float> v2{1, 4.1, 5};
  ASSERT_TRUE((v1 + v2).equal({3, 5.1, 8}));
}

TEST(Plane, getIntersectionLine) {
  Plane<float> p1{{16, 7, 8}, {7.8, 19100, 89}, {100, 2.2, 4.3}};
  Plane<float> p2{{16, 7, 8}, {7.8, 19100, 89}, {0, 0, 0}};
  Line3D<float> intersection = p1.getIntersectionLine(p2);
  Line3D<float> intersection_ref{{8.2, 19093, 81}, {7.8, 19100, 89}};

  ASSERT_TRUE(intersection.valid());
  ASSERT_TRUE(intersection.equal(intersection_ref));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
