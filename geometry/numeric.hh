#ifndef TRIANGLES_3D_GEOMETRY_NUMERIC_HH_
#define TRIANGLES_3D_GEOMETRY_NUMERIC_HH_

#include <cmath>

namespace geometry {

namespace numeric {

// T stands for any floating point type
template <typename T>
bool isClose(T x, T y) {
  return std::fabs(x - y) < 1e-4;
}

}

}

#endif // TRIANGLES_3D_GEOMETRY_NUMERIC_HH_
