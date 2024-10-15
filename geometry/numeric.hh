#ifndef TRIANGLES_3D_GEOMETRY_NUMERIC_HH_
#define TRIANGLES_3D_GEOMETRY_NUMERIC_HH_

#include <cmath>

namespace geometry {

namespace numeric {

// T stands for any floating point type
template <typename T>
class Limits {
 public:
  static T tolerance();
};

template <>
class Limits<float> {
 public:
  static float tolerance() {
    return 1e-4;
  }
};

template <>
class Limits<double> {
 public:
  static double tolerance() {
    return 1e-7;
  }
};

template <typename T>
bool equal(T x, T y) {
  return std::fabs(x - y) <= Limits<T>::tolerance();
}

}

}

#endif // TRIANGLES_3D_GEOMETRY_NUMERIC_HH_
