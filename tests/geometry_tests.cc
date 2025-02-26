#include <gtest/gtest.h>

#include "geom/vector3d.hh"
#include "geom/plane.hh"
#include "geom/triangle3d.hh"

using namespace geometry;

TEST(Vector3D, add) {
  Vector3D<double> v1{2, 1, 3};
  Vector3D<double> v2{1, 4.1, 5};
  ASSERT_TRUE((v1 + v2).equal({3, 5.1, 8}));
}

TEST(Vector3D, dot) {
  Vector3D<double> a{1, 2, 3};
  Vector3D<double> b{5, 6, 7};
  ASSERT_TRUE(comparator::isClose(dot(a, b), 38.0));
}

TEST(Vector3D, crossProduct) {
  Vector3D<double> a{1, 0, 0};
  Vector3D<double> b{0, 1, 0};
  ASSERT_TRUE(crossProduct(a, b).equal({0, 0, 1}));
  ASSERT_TRUE(crossProduct(b, a).equal({0, 0, -1}));
}

TEST(Vector3D, normalize) {
  Vector3D<double> v{1000, 99, 9.7};
  auto n = v.normalize();
  ASSERT_TRUE(n.equal(1/std::sqrt(dot(v, v))*v));
  ASSERT_TRUE(comparator::isClose(n.norm(), 1.0));
}

TEST(Line3D, equal) {
  Line3D<double> l1{{0, 0, 1}, {7, 8, 9}};
  Line3D<double> l2{{0, 0, -10000}, {7, 8, 9}};
  ASSERT_TRUE(l1.equal(l2));
}

TEST(Plane, constructor) {
  Plane<double> p1{{16, 7, 8}, {7.8, 19100, 89}, {100, 2.2, 4.3}};
  Plane<double> p2{{0, 0, 0}, {1, 0, 0}, {-1000, 0, 0}};
  ASSERT_TRUE(p1.valid());
  ASSERT_FALSE(p2.valid());

  ASSERT_TRUE(comparator::isClose(p1.normal().norm(), 1.0));
}

TEST(Plane, getIntersectionLine) {
  Plane<double> p1{{16, 7, 8}, {7.8, 19, 89}, {100, 2.2, 4.3}};
  Plane<double> p2{{16, 7, 8}, {7.8, 19, 89}, {0, 0, 0}};
  Line3D<double> intersection = p1.getIntersectionLine(p2);
  Line3D<double> intersection_ref{{-8.2, 12, 81}, {7.8, 19, 89}};

  ASSERT_TRUE(intersection.valid());
  ASSERT_TRUE(intersection.equal(intersection_ref));
}

TEST(Triangle3D, intersects) {
  Triangle3D<double> t1{{2, 4, 5}, {1, 2, 3}, {9, 19, 1000}};
  Triangle3D<double> t2{{0, 0, 0}, {2, 4, 5}, {8, 8, 52}};

  ASSERT_TRUE(t1.intersects(t2));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
