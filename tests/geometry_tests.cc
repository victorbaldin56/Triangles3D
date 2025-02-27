#include <gtest/gtest.h>

#include "geom/plane.hh"
#include "geom/triangle3d.hh"
#include "geom/vector3d.hh"

using namespace geometry;

constexpr double kAbsTol = 0;
constexpr double kRelTol = 1e-3;

TEST(Vector3D, add) {
  Vector3D<double> v1{2, 1, 3};
  Vector3D<double> v2{1, 4.1, 5};
  ASSERT_TRUE((v1 + v2).isClose({3, 5.1, 8}));
}

TEST(Vector3D, dot) {
  Vector3D<double> a{1, 2, 3};
  Vector3D<double> b{5, 6, 7};
  ASSERT_TRUE(comparator::isClose(dot(a, b), 38.0));
}

TEST(Vector3D, crossProduct) {
  Vector3D<double> a{1, 0, 0};
  Vector3D<double> b{0, 1, 0};
  ASSERT_TRUE(crossProduct(a, b).isClose({0, 0, 1}));
  ASSERT_TRUE(crossProduct(b, a).isClose({0, 0, -1}));
}

TEST(Vector3D, normalize) {
  Vector3D<double> v{1000, 99, 9.7};
  auto n = v.normalize();
  ASSERT_TRUE(n.isClose(1 / std::sqrt(dot(v, v)) * v));
  ASSERT_TRUE(comparator::isClose(n.norm(), 1.0));
}

TEST(Line3D, isClose) {
  Line3D<double> l1{{0, 0, 1}, {7, 8, 9}};
  Line3D<double> l2{{0, 0, -10000}, {7, 8, 9}};
  ASSERT_TRUE(l1.isClose(l2));
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
  ASSERT_TRUE(intersection.isClose(intersection_ref));
}

TEST(Plane, getIntersectionPoint) {
  Plane<double> p{{0, 0, 0}, {1, 0, 0}, {1, 1, 1}};
  Segment3D<double> s1{{5, -1, 0}, {7, 1, 0}};
  Segment3D<double> s2{{0, 0, 1}, {0, 1, 2}};

  auto point0 = p.getIntersectionPoint(s1.line());
  auto point1 = p.getIntersectionPoint(s1);
  ASSERT_TRUE(point0.isClose({6, 0, 0}));
  ASSERT_TRUE(point1.valid());
  ASSERT_TRUE(point1.isClose({6, 0, 0}));

  auto point2 = p.getIntersectionPoint(s2);
  ASSERT_FALSE(point2.valid());
}

TEST(Triangle3D, Intersects_SimpleCase) {
  Triangle3D<double> t1{{0.3, 0.7, 1.2}, {1.5, 0.4, 2.1}, {0.8, 1.9, 3.4}};
  Triangle3D<double> t2{{0.3, 0.7, 1.2}, {1.5, 0.4, 2.1}, {0.8, 1.9, 3.4}};

  ASSERT_TRUE(t1.intersects(t2, kAbsTol, kRelTol));  // Identical triangles should intersect
}

TEST(Triangle3D, Intersects_EdgeCase) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {1.1, 0.2, 0.3}, {0.1, 1.2, 0.3}};
  Triangle3D<double> t2{{1.1, 0.2, 0.3}, {0.1, 1.2, 0.3}, {1.1, 1.2, 0.3}};

  ASSERT_TRUE(t1.intersects(t2, kAbsTol, kRelTol));  // Triangles share an edge
}

TEST(Triangle3D, Intersects_NoIntersection) {
  Triangle3D<double> t1{{0.5, 0.5, 0.5}, {1.5, 0.5, 0.5}, {0.5, 1.5, 0.5}};
  Triangle3D<double> t2{{2.2, 2.2, 2.2}, {3.3, 2.2, 2.2}, {2.2, 3.3, 2.2}};

  ASSERT_FALSE(t1.intersects(t2, kAbsTol, kRelTol));  // Triangles are far apart
}

TEST(Triangle3D, Intersects_CoplanarButSeparate) {
  Triangle3D<double> t1{{0.2, 0.3, 0.4}, {1.2, 0.3, 0.4}, {0.2, 1.3, 0.4}};
  Triangle3D<double> t2{{2.2, 0.3, 0.4}, {3.2, 0.3, 0.4}, {2.2, 1.3, 0.4}};

  ASSERT_FALSE(t1.intersects(t2, kAbsTol, kRelTol));  // Coplanar but separate triangles
}

TEST(Triangle3D, Intersects_OnePointInCommon) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {1.1, 0.2, 0.3}, {0.1, 1.2, 0.3}};
  Triangle3D<double> t2{{0.1, 0.2, 0.3}, {-1.1, 0.2, 0.3}, {0.1, -1.2, 0.3}};

  ASSERT_TRUE(t1.intersects(t2, kAbsTol, kRelTol));  // Triangles share one vertex
}

TEST(Triangle3D, Intersects_IntersectingAtEdge) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {2.1, 0.2, 0.3}, {0.1, 2.2, 0.3}};
  Triangle3D<double> t2{{1.1, 1.2, 0.3}, {3.1, 1.2, 0.3}, {1.1, 3.2, 0.3}};

  ASSERT_TRUE(t1.intersects(t2, kAbsTol, kRelTol));  // Triangles intersect along an edge
}

#if 1
TEST(Triangle3D, Intersects_IntersectingAtSinglePoint) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {2.1, 0.2, 0.3}, {0.1, 2.2, 0.3}};
  Triangle3D<double> t2{{1.1, 1.2, 1.3}, {1.1, 1.2, -1.3}, {2.2, 2.2, 0.3}};

  ASSERT_TRUE(t1.intersects(t2, kAbsTol, kRelTol));  // Triangles intersect at a single point
}
#endif

TEST(Triangle3D, Intersects_OneInsideTheOther) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {2.1, 0.2, 0.3}, {0.1, 2.2, 0.3}};
  Triangle3D<double> t2{{0.5, 0.6, 0.3}, {1.5, 0.6, 0.3}, {0.5, 1.6, 0.3}};

  ASSERT_TRUE(t1.intersects(t2, kAbsTol, kRelTol));  // One triangle is entirely inside the other
}

TEST(Triangle3D, Intersects_NonCoplanarIntersection) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {2.1, 0.2, 0.3}, {0.1, 2.2, 0.3}};
  Triangle3D<double> t2{{1.1, 1.2, -1.3}, {1.1, 1.2, 1.3}, {2.2, 2.2, 0.3}};

  ASSERT_TRUE(t1.intersects(t2, kAbsTol, kRelTol));  // Non-coplanar triangles intersecting
}

TEST(Triangle3D, Intersects_NonCoplanarNoIntersection) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {2.1, 0.2, 0.3}, {0.1, 2.2, 0.3}};
  Triangle3D<double> t2{{3.3, 3.4, 1.5}, {4.4, 3.4, 1.5}, {3.3, 4.4, 1.5}};

  ASSERT_FALSE(
      t1.intersects(t2, kAbsTol, kRelTol));  // Non-coplanar triangles with no intersection
}

#if 1
TEST(Triangle3D, Intersects_DegenerateTriangle) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {0.1, 0.2, 0.3}, {0.1, 0.2, 0.3}};  // Degenerate triangle (a point)
  Triangle3D<double> t2{{0.1, 0.2, 0.3}, {1.1, 0.2, 0.3}, {0.1, 1.2, 0.3}};

  ASSERT_TRUE(t1.intersects(t2, kAbsTol, kRelTol));  // Degenerate triangle intersects at a point
}
#endif

TEST(Triangle3D, Intersects_DegenerateTriangleNoIntersection) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {0.1, 0.2, 0.3},
                        {0.1, 0.2, 0.3}};  // Degenerate triangle (a point)
  Triangle3D<double> t2{{1.1, 1.2, 1.3}, {2.2, 2.2, 2.3}, {3.3, 3.3, 3.3}};

  ASSERT_FALSE(t1.intersects(t2, kAbsTol, kRelTol));  // Degenerate triangle does not intersect
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
