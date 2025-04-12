#pragma once

#include <cstdlib>
#include <iomanip>
#include <iostream>

#ifdef NDEBUG
#define GLHPP_ASSERT_EQ(a, b) 0
#else
#define GLHPP_ASSERT_EQ(a, b)                                                \
  (a != b)                                                                   \
      ? (std::cerr << "Assertion failed: expected equality of " #a " ("      \
                   << std::hex << "0x" << a << "), and " #b " (" << std::hex \
                   << "0x" << b << "), "                                     \
                   << "function " << __FUNCTION__ << ", file " << __FILE__   \
                   << ", line " << __LINE__ << "." << std::endl              \
                   << std::endl,                                             \
         std::abort(), 0)                                                    \
      : 1
#endif
