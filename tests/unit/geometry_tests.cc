#include "CGAL/Exact_predicates_exact_constructions_kernel.h"
#include "CGAL/intersections.h"

#include "gtest/gtest.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "geom/plane.hh"
#include "geom/triangle3d.hh"
#include "geom/vector3d.hh"

using namespace geometry;

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
using Triangle_3 = Kernel::Triangle_3;
using Point_3 = Kernel::Point_3;

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

TEST(Segment3D, Intersects_3D_SimpleIntersection) {
  // Two segments intersecting at a single point in 3D space
  Segment3D<double> seg1{{0.0, 0.0, 0.0}, {2.0, 2.0, 2.0}};
  Segment3D<double> seg2{{0.0, 2.0, 0.0}, {2.0, 0.0, 2.0}};

  ASSERT_TRUE(seg1.intersects(seg2));  // Segments intersect at (1.0, 1.0, 1.0)
}

TEST(Segment3D, Intersects_3D_NoIntersection) {
  // Two segments in 3D space that do not intersect
  Segment3D<double> seg1{{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}};
  Segment3D<double> seg2{{2.0, 2.0, 2.0}, {3.0, 3.0, 3.0}};

  ASSERT_FALSE(seg1.intersects(seg2));  // Segments are parallel and disjoint
}

TEST(Segment3D, Intersects_3D_CollinearNoOverlap) {
  // Two collinear segments in 3D space with no overlap
  Segment3D<double> seg1{{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}};
  Segment3D<double> seg2{{2.0, 2.0, 2.0}, {3.0, 3.0, 3.0}};

  ASSERT_FALSE(seg1.intersects(seg2));  // Segments are collinear but disjoint
}

TEST(Segment3D, Intersects_3D_CollinearOverlap) {
  // Two collinear segments in 3D space with overlap
  Segment3D<double> seg1{{0.0, 0.0, 0.0}, {2.0, 2.0, 2.0}};
  Segment3D<double> seg2{{1.0, 1.0, 1.0}, {3.0, 3.0, 3.0}};

  ASSERT_TRUE(seg1.intersects(seg2));
}

TEST(Segment3D, Intersects_3D_SharedEndpoint) {
  // Two segments in 3D space sharing an endpoint
  Segment3D<double> seg1{{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}};
  Segment3D<double> seg2{{1.0, 1.0, 1.0}, {2.0, 2.0, 2.0}};

  ASSERT_TRUE(seg1.intersects(seg2));
}

TEST(Segment3D, Intersects_3D_ParallelNoIntersection) {
  // Two parallel segments in 3D space that do not intersect
  Segment3D<double> seg1{{0.0, 0.0, 0.0}, {1.0, 1.0, 0.0}};
  Segment3D<double> seg2{{0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}};

  ASSERT_FALSE(seg1.intersects(seg2));  // Segments are parallel and disjoint
}

TEST(Segment3D, Intersects_3D_SkewNoIntersection) {
  // Two skew segments in 3D space that do not intersect
  Segment3D<double> seg1{{0.0, 0.0, 0.0}, {1.0, 1.0, 0.0}};
  Segment3D<double> seg2{{0.0, 1.0, 1.0}, {1.0, 0.0, 1.0}};

  ASSERT_FALSE(seg1.intersects(seg2));
}

TEST(Segment3D, Intersects_3D_DegenerateSegment) {
  // One segment is degenerate (a single point)
  Segment3D<double> seg1{{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}};
  Segment3D<double> seg2{{0.0, 0.0, 0.0}, {2.0, 2.0, 2.0}};

  ASSERT_TRUE(seg1.intersects(seg2));  // Degenerate segment lies on seg2
}

TEST(Segment3D, Intersects_3D_BothDegenerate) {
  // Both segments are degenerate (single points)
  Segment3D<double> seg1{{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}};
  Segment3D<double> seg2{{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}};

  ASSERT_TRUE(seg1.intersects(seg2));  // Both segments are the same point
}

TEST(Segment3D, Intersects_3D_DegenerateNoIntersection) {
  // Both segments are degenerate (single points) but different
  Segment3D<double> seg1{{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}};
  Segment3D<double> seg2{{2.0, 2.0, 2.0}, {2.0, 2.0, 2.0}};

  ASSERT_FALSE(seg1.intersects(seg2));  // Segments are different points
}

TEST(Triangle3D, Intersects_SimpleCase) {
  Triangle3D<double> t1{{0.3, 0.7, 1.2}, {1.5, 0.4, 2.1}, {0.8, 1.9, 3.4}};
  Triangle3D<double> t2{{0.3, 0.7, 1.2}, {1.5, 0.4, 2.1}, {0.8, 1.9, 3.4}};

  ASSERT_TRUE(t1.intersects(t2));  // Identical triangles should intersect
}

TEST(Triangle3D, Intersects_EdgeCase) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {1.1, 0.2, 0.3}, {0.1, 1.2, 0.3}};
  Triangle3D<double> t2{{1.1, 0.2, 0.3}, {0.1, 1.2, 0.3}, {1.1, 1.2, 0.3}};

  ASSERT_TRUE(t1.intersects(t2));  // Triangles share an edge
}

TEST(Triangle3D, Intersects_NoIntersection) {
  Triangle3D<double> t1{{0.5, 0.5, 0.5}, {1.5, 0.5, 0.5}, {0.5, 1.5, 0.5}};
  Triangle3D<double> t2{{2.2, 2.2, 2.2}, {3.3, 2.2, 2.2}, {2.2, 3.3, 2.2}};

  ASSERT_FALSE(t1.intersects(t2));  // Triangles are far apart
}

TEST(Triangle3D, Intersects_CoplanarButSeparate) {
  Triangle3D<double> t1{{0.2, 0.3, 0.4}, {1.2, 0.3, 0.4}, {0.2, 1.3, 0.4}};
  Triangle3D<double> t2{{2.2, 0.3, 0.4}, {3.2, 0.3, 0.4}, {2.2, 1.3, 0.4}};

  ASSERT_FALSE(t1.intersects(t2));  // Coplanar but separate triangles
}

TEST(Triangle3D, Intersects_OnePointInCommon) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {1.1, 0.2, 0.3}, {0.1, 1.2, 0.3}};
  Triangle3D<double> t2{{0.1, 0.2, 0.3}, {-1.1, 0.2, 0.3}, {0.1, -1.2, 0.3}};

  ASSERT_TRUE(t1.intersects(t2));  // Triangles share one vertex
}

TEST(Triangle3D, Intersects_IntersectingAtEdge) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {2.1, 0.2, 0.3}, {0.1, 2.2, 0.3}};
  Triangle3D<double> t2{{1.1, 1.2, 0.3}, {3.1, 1.2, 0.3}, {1.1, 3.2, 0.3}};

  ASSERT_TRUE(t1.intersects(t2));  // Triangles intersect along an edge
}

TEST(Triangle3D, Intersects_IntersectingAtSinglePoint) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {2.1, 0.2, 0.3}, {0.1, 2.2, 0.3}};
  Triangle3D<double> t2{{1.1, 1.2, 1.3}, {1.1, 1.2, -1.3}, {2.2, 2.2, 0.3}};

  ASSERT_TRUE(t1.intersects(t2));  // Triangles intersect at a single point
}

TEST(Triangle3D, Intersects_OneInsideTheOther) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {2.1, 0.2, 0.3}, {0.1, 2.2, 0.3}};
  Triangle3D<double> t2{{0.5, 0.6, 0.3}, {1.5, 0.6, 0.3}, {0.5, 1.6, 0.3}};

  ASSERT_TRUE(t1.intersects(t2));  // One triangle is entirely inside the other
}

TEST(Triangle3D, Intersects_NonCoplanarIntersection) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3}, {2.1, 0.2, 0.3}, {0.1, 2.2, 0.3}};
  Triangle3D<double> t2{{1.1, 1.2, -1.3}, {1.1, 1.2, 1.3}, {2.2, 2.2, 0.3}};

  ASSERT_TRUE(t1.intersects(t2));  // Non-coplanar triangles intersecting
}

TEST(Triangle3D, Intersects_NonCoplanar_ParallelTriangles) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {2.1, 0.2, 0.3},
                        {0.1, 2.2, 0.3}};  // Triangle in XY plane
  Triangle3D<double> t2{{0.1, 0.2, 1.3},
                        {2.1, 0.2, 1.3},
                        {0.1, 2.2, 1.3}};  // Parallel triangle above XY plane

  ASSERT_FALSE(t1.intersects(t2));
}

TEST(Triangle3D, Intersects_NonCoplanarNoIntersection) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {2.1, 0.2, 0.3},
                        {0.1, 2.2, 0.3}};
  Triangle3D<double> t2{{3.3, 3.4, 1.5},
                        {4.4, 3.4, 1.5},
                        {3.3, 4.4, 1.5}};

  ASSERT_FALSE(
      t1.intersects(t2));  // Non-coplanar triangles with no intersection
}

TEST(Triangle3D, Intersects_NonCoplanar_IntersectingAlongLine) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {2.1, 0.2, 0.3},
                        {0.1, 2.2, 0.3}};  // Triangle in XY plane
  Triangle3D<double> t2{{1.1, 1.2, -1.3},
                        {1.1, 1.2, 1.3},
                        {1.1, 1.2, 0.3}};  // Vertical line intersecting t1

  ASSERT_TRUE(t1.intersects(t2));  // Triangles intersect along a line
}

TEST(Triangle3D, Intersects_NonCoplanar_OneTrianglePiercesTheOther) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {2.1, 0.2, 0.3},
                        {0.1, 2.2, 0.3}};  // Triangle in XY plane
  Triangle3D<double> t2{{1.1, 1.2, -1.3},
                        {1.1, 1.2, 1.3},
                        {2.2, 2.2, 0.3}};  // Triangle piercing t1

  ASSERT_TRUE(t1.intersects(t2));  // One triangle pierces the other
}

TEST(Triangle3D, Intersects_NonCoplanar_IntersectingAtEdge) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {2.1, 0.2, 0.3},
                        {0.1, 2.2, 0.3}};  // Triangle in XY plane
  Triangle3D<double> t2{{2.1, 0.2, 0.3},
                        {2.1, 0.2, 1.3},
                        {0.1, 2.2, 0.3}};  // Triangle sharing an edge with t1

  ASSERT_TRUE(t1.intersects(t2));  // Triangles intersect at an edge
}

TEST(Triangle3D, Intersects_NonCoplanar_IntersectingAtVertex) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {2.1, 0.2, 0.3},
                        {0.1, 2.2, 0.3}};  // Triangle in XY plane
  Triangle3D<double> t2{{0.1, 0.2, 0.3},
                        {0.1, 0.2, 1.3},
                        {1.1, 1.2, 0.3}};  // Triangle sharing a vertex with t1

  ASSERT_TRUE(t1.intersects(t2));  // Triangles intersect at a vertex
}

TEST(Triangle3D, Intersects_NonCoplanar_WideIntersection) {
  Triangle3D<double> t1{{0.0, 0.0, 0.0},
                        {1.0, 0.0, 0.0},
                        {0.0, 1.0, 0.0}};
  Triangle3D<double> t2{{0.25, 0.25, 1.0},
                        {0.25, 0.25, -1.0},
                        {0.25, 0.25, 0.0}};

  ASSERT_TRUE(t1.intersects(t2));
}

TEST(Triangle3D, Intersects_NonCoplanar_GeoGebra_NoIntersection1) {
  Triangle3D<double> t1{{-11.24638, 8.98911, 2},
                        {0, 0, -5},
                        {0, 100, 0}};
  Triangle3D<double> t2{{-1.934114, -2.56997, -1},
                        {-5.02241, 1.85908, 0},
                        {0, 0, 5}};
  Triangle_3 cgal_t1(Point_3(t1.a_.x_, t1.a_.y_, t1.a_.z_),
                     Point_3(t1.b_.x_, t1.b_.y_, t1.b_.z_),
                     Point_3(t1.c_.x_, t1.c_.y_, t1.c_.z_));
  Triangle_3 cgal_t2(Point_3(t2.a_.x_, t2.a_.y_, t2.a_.z_),
                     Point_3(t2.b_.x_, t2.b_.y_, t2.b_.z_),
                     Point_3(t2.c_.x_, t2.c_.y_, t2.c_.z_));

  ASSERT_FALSE(CGAL::do_intersect(cgal_t1, cgal_t2));
  ASSERT_FALSE(t1.intersects(t2));
}

TEST(Triangle3D, Intersects_NonCoplanar_GeoGebra_Intersection1) {
  Triangle3D<double> t1{{-14.23772, -2.9921, 0},
                        {-15, -15, -5},
                        {-11.20053, 23.11501, 0}};
  Triangle3D<double> t2{{-28.99101, 14.7833, 0},
                        {-10.71675, -2.03107, -1},
                        {-15, -20, 2.1}};
  Triangle_3 cgal_t1(Point_3(t1.a_.x_, t1.a_.y_, t1.a_.z_),
                     Point_3(t1.b_.x_, t1.b_.y_, t1.b_.z_),
                     Point_3(t1.c_.x_, t1.c_.y_, t1.c_.z_));
  Triangle_3 cgal_t2(Point_3(t2.a_.x_, t2.a_.y_, t2.a_.z_),
                     Point_3(t2.b_.x_, t2.b_.y_, t2.b_.z_),
                     Point_3(t2.c_.x_, t2.c_.y_, t2.c_.z_));

  ASSERT_TRUE(CGAL::do_intersect(cgal_t1, cgal_t2));
  ASSERT_TRUE(t1.intersects(t2));
}

TEST(Triangle3D, Intersects_DegenerateTriangle) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {0.1, 0.2, 0.3},
                        {0.1, 0.2, 0.3}};  // Degenerate triangle (a point)
  Triangle3D<double> t2{{0.1, 0.2, 0.3},
                        {1.1, 0.2, 0.3},
                        {0.1, 1.2, 0.3}};

  ASSERT_TRUE(t1.intersects(t2));  // Degenerate triangle intersects at a point
}

TEST(Triangle3D, Intersects_DegenerateTriangleNoIntersection) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {0.1, 0.2, 0.3},
                        {0.1, 0.2, 0.3}};  // Degenerate triangle (a point)
  Triangle3D<double> t2{{1.1, 1.2, 1.3},
                        {2.2, 2.2, 2.3},
                        {3.3, 3.3, 3.3}};

  ASSERT_FALSE(t1.intersects(t2));  // Degenerate triangle does not intersect
}

TEST(Triangle3D, Intersects_BothDegenerateToPoint_SamePoint) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {0.1, 0.2, 0.3},
                        {0.1, 0.2, 0.3}};  // Degenerate to a point
  Triangle3D<double> t2{{0.1, 0.2, 0.3},
                        {0.1, 0.2, 0.3},
                        {0.1, 0.2, 0.3}};  // Degenerate to the same point

  ASSERT_TRUE(t1.intersects(t2));  // Both triangles are the same point
}

TEST(Triangle3D, Intersects_BothDegenerateToPoint_DifferentPoints) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {0.1, 0.2, 0.3},
                        {0.1, 0.2, 0.3}};  // Degenerate to a point
  Triangle3D<double> t2{{1.1, 1.2, 1.3},
                        {1.1, 1.2, 1.3},
                        {1.1, 1.2, 1.3}};  // Degenerate to a different point

  ASSERT_FALSE(t1.intersects(t2));  // Different points, no intersection
}

TEST(Triangle3D, Intersects_BothDegenerateToSegment_SameSegment) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {1.1, 1.2, 1.3},
                        {0.1, 0.2, 0.3}};  // Degenerate to a segment
  Triangle3D<double> t2{{0.1, 0.2, 0.3},
                        {1.1, 1.2, 1.3},
                        {0.1, 0.2, 0.3}};  // Degenerate to the same segment

  ASSERT_TRUE(t1.intersects(t2));  // Both triangles are the same segment
}

TEST(Triangle3D, Intersects_BothDegenerateToSegment_OverlappingSegments) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {1.1, 1.2, 1.3},
                        {0.1, 0.2, 0.3}};  // Degenerate to a segment
  Triangle3D<double> t2{{0.5, 0.6, 0.7},
                        {1.1, 1.2, 1.3},
                        {0.5, 0.6, 0.7}};

  ASSERT_TRUE(t1.intersects(t2));  // Segments overlap
}

TEST(Triangle3D, Intersects_BothDegenerateToSegment_NonOverlappingSegments) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {1.1, 1.2, 1.3},
                        {0.1, 0.2, 0.3}};  // Degenerate to a segment
  Triangle3D<double> t2{{2.2, 2.3, 2.4},
                        {3.3, 3.4, 3.5},
                        {2.2, 2.3, 2.4}};

  ASSERT_FALSE(t1.intersects(t2));  // Segments do not overlap
}

TEST(Triangle3D, Intersects_BothDegenerateToSegment_SharedEndpoint) {
  Triangle3D<double> t1{{0.1, 0.2, 0.3},
                        {1.1, 1.2, 1.3},
                        {0.1, 0.2, 0.3}};  // Degenerate to a segment
  Triangle3D<double> t2{{1.1, 1.2, 1.3},
                        {2.2, 2.3, 2.4},
                        {1.1, 1.2, 1.3}};

  ASSERT_TRUE(t1.intersects(t2));  // Segments share an endpoint
}

TEST(Triangle3D,
     Intersects_OneDegenerateToPoint_OneDegenerateToSegment_PointOnSegment) {
  Triangle3D<double> t1{{0.5, 0.6, 0.7},
                        {0.5, 0.6, 0.7},
                        {0.5, 0.6, 0.7}};  // Degenerate to a point
  Triangle3D<double> t2{{0.1, 0.2, 0.3},
                        {1.1, 1.2, 1.3},
                        {0.1, 0.2, 0.3}};  // Degenerate to a segment

  ASSERT_TRUE(t1.intersects(t2));  // Point lies on the segment
}

TEST(
    Triangle3D,
    Intersects_OneDegenerateToPoint_OneDegenerateToSegment_PointNotOnSegment) {
  Triangle3D<double> t1{{2.2, 2.3, 2.4},
                        {2.2, 2.3, 2.4},
                        {2.2, 2.3, 2.4}};  // Degenerate to a point
  Triangle3D<double> t2{{0.1, 0.2, 0.3},
                        {1.1, 1.2, 1.3},
                        {0.1, 0.2, 0.3}};  // Degenerate to a segment

  ASSERT_FALSE(t1.intersects(t2));  // Point does not lie on the segment
}

TEST(Triangle3D, Intersects_CoplanarEdgeIntersectionNoVertexContainment) {
  Triangle3D<double> t1{{ 0.0,  2.0, 0.0},
                        {-1.0, -1.0, 0.0},
                        { 1.0, -1.0, 0.0}};
  Triangle3D<double> t2{{ 0.0, -2.0, 0.0},
                        {-1.0,  1.0, 0.0},
                        { 1.0,  1.0, 0.0}};

  ASSERT_FALSE(t1.contains(t2.a_));
  ASSERT_FALSE(t1.contains(t2.b_));
  ASSERT_FALSE(t1.contains(t2.c_));

  ASSERT_FALSE(t2.contains(t1.a_));
  ASSERT_FALSE(t2.contains(t1.b_));
  ASSERT_FALSE(t2.contains(t1.c_));

  ASSERT_TRUE(t1.intersectsEdges(t2));
  ASSERT_TRUE(t1.intersects(t2));
}

int main(int argc, char** argv) {
  spdlog::set_level(spdlog::level::trace);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
