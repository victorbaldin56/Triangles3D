#include <gtest/gtest.h>

#include "geom/vector3d.hh"
#include "geom/plane.hh"
#include "geom/triangle3d.hh"

using namespace geometry;

TEST(Vector3D, add) {
  Vector3D<float> v1{2, 1, 3};
  Vector3D<float> v2{1, 4.1, 5};
  ASSERT_TRUE((v1 + v2).equal({3, 5.1, 8}));
}

TEST(Vector3D, dot) {
  Vector3D<float> a{1, 2, 3};
  Vector3D<float> b{5, 6, 7};
  ASSERT_TRUE(comparator::isClose(dot(a, b), 38.f));
}

TEST(Vector3D, crossProduct) {
  Vector3D<float> a{1, 0, 0};
  Vector3D<float> b{0, 1, 0};
  ASSERT_TRUE(crossProduct(a, b).equal({0, 0, 1}));
  ASSERT_TRUE(crossProduct(b, a).equal({0, 0, -1}));
}

TEST(Vector3D, normalize) {
  Vector3D<float> v{1000, 99, 9.7};
  auto n = v.normalize();
  ASSERT_TRUE(n.equal(1/std::sqrt(dot(v, v))*v));
  ASSERT_TRUE(comparator::isClose(n.norm(), 1.f));
}

TEST(Line3D, equal) {
  Line3D<float> l1{{0, 0, 1}, {7, 8, 9}};
  Line3D<float> l2{{0, 0, -10000}, {7, 8, 9}};
  ASSERT_TRUE(l1.equal(l2));
}

TEST(Plane, constructor) {
  Plane<float> p1{{16, 7, 8}, {7.8, 19100, 89}, {100, 2.2, 4.3}};
  Plane<float> p2{{0, 0, 0}, {1, 0, 0}, {-1000, 0, 0}};
  ASSERT_TRUE(p1.valid());
  ASSERT_FALSE(p2.valid());

  ASSERT_TRUE(comparator::isClose(p1.normal().norm(), 1.f));
}

TEST(Plane, getIntersectionLine) {
  Plane<float> p1{{16, 7, 8}, {7.8, 19, 89}, {100, 2.2, 4.3}};
  Plane<float> p2{{16, 7, 8}, {7.8, 19, 89}, {0, 0, 0}};
  Line3D<float> intersection = p1.getIntersectionLine(p2);
  Line3D<float> intersection_ref{{-8.2, 12, 81}, {7.8, 19, 89}};

  ASSERT_TRUE(intersection.valid());
  ASSERT_TRUE(intersection.equal(intersection_ref));
}

TEST(Triangle3D, intersects) {
  Triangle3D<float> t1{{2, 4, 5}, {1, 2, 3}, {9, 19, 1000}};
  Triangle3D<float> t2{{0, 0, 0}, {2, 4, 5}, {8, 8, 52}};

  ASSERT_TRUE(t1.intersects(t2));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
